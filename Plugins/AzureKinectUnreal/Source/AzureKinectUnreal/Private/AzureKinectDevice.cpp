// Fill out your copyright notice in the Description page of Project Settings.


#include "AzureKinectDevice.h"
#include "Containers/Ticker.h"

DEFINE_LOG_CATEGORY(AzureKinectDeviceLog);

AzureKinectDevice::AzureKinectDevice() :
	AzureKinectDevice(0, 0)
{
}

AzureKinectDevice::AzureKinectDevice(int32 Id, int32 TimeOut) :
	NativeKinectDevice(nullptr),	//NULL
	DeviceId(Id),
	NativeBodyTracker(nullptr),	//NULL
	TimeOutInMilliSecs(TimeOut),
	Thread(nullptr),
	bIsInitialized(false)
{
}

AzureKinectDevice::~AzureKinectDevice()
{
	Shutdown();
}

bool AzureKinectDevice::Initialize(k4a_depth_mode_t DepthMode)
{
	// Open the Azure Kinect Device
	k4a_result_t deviceOpenResult = k4a_device_open(DeviceId, &NativeKinectDevice);
	if (deviceOpenResult != K4A_RESULT_SUCCEEDED)
	{
		UE_LOG(AzureKinectDeviceLog, Error, TEXT("Open Kinect device (id : %d) Failed!"), DeviceId);
		return false;
	}

	// Start the Camera and make sure the Depth Camera is Enabled
	k4a_device_configuration_t deviceConfig = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	deviceConfig.depth_mode = DepthMode;
	deviceConfig.color_resolution = K4A_COLOR_RESOLUTION_OFF;

	k4a_result_t deviceStartCameraResult = k4a_device_start_cameras(NativeKinectDevice, &deviceConfig);
	if (deviceStartCameraResult != K4A_RESULT_SUCCEEDED)
	{
		UE_LOG(AzureKinectDeviceLog, Error, TEXT("Kinect device (id : %d) start camera Failed!"), DeviceId);
		return false;
	}

	k4a_calibration_t sensorCalibration;
	k4a_result_t deviceCalibrationResult = k4a_device_get_calibration(NativeKinectDevice, deviceConfig.depth_mode, deviceConfig.color_resolution, &sensorCalibration);
	if (deviceCalibrationResult != K4A_RESULT_SUCCEEDED)
	{
		UE_LOG(AzureKinectDeviceLog, Error, TEXT("Kinect device (id : %d) get depth camera calibration Failed!"), DeviceId);
		return false;
	}

	k4abt_tracker_configuration_t trackerConfig = K4ABT_TRACKER_CONFIG_DEFAULT;
	k4a_result_t trackerCreateResult = k4abt_tracker_create(&sensorCalibration, trackerConfig, &NativeBodyTracker);
	if (trackerCreateResult != K4A_RESULT_SUCCEEDED)
	{
		UE_LOG(AzureKinectDeviceLog, Error, TEXT("Kinect device (id : %d) - Body Tracker creation Failed!"), DeviceId);
		return false;
	}

	bIsInitialized = true;

	InitializeBodies();
	StartKinectThread();

	TickHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &AzureKinectDevice::OnTick), TickInterval);

	return true;
}

void AzureKinectDevice::Shutdown()
{
	bIsInitialized = false;

	if (Thread)
	{
		Thread->Shutdown();
		Thread = nullptr;
	}

	FTicker::GetCoreTicker().RemoveTicker(TickHandle);

	for (UAzureKinectBody *body : Bodies)
	{
		body->RemoveFromRoot();
	}
	Bodies.Empty();

	if (NativeBodyTracker)
	{
		k4abt_tracker_shutdown(NativeBodyTracker);
		k4abt_tracker_destroy(NativeBodyTracker);
		NativeBodyTracker = nullptr;
		UE_LOG(AzureKinectDeviceLog, Warning, TEXT("BodyTracker is Shutdown and Destroyed"));
	}

	if (NativeKinectDevice)
	{
		k4a_device_stop_cameras(NativeKinectDevice);
		k4a_device_close(NativeKinectDevice);
		NativeKinectDevice = nullptr;
		UE_LOG(AzureKinectDeviceLog, Warning, TEXT("KinectDevice Camera is Stopped and Closed"));
	}
}

void AzureKinectDevice::CaptureBodyTrackingFrame()
{
	if (!NativeKinectDevice)
	{
		UE_LOG(AzureKinectDeviceLog, Error, TEXT("Kinect device for capturing body tracking frame is Invalid!"));
		return;
	}

	if (!NativeBodyTracker)
	{
		UE_LOG(AzureKinectDeviceLog, Error, TEXT("Body Tracker for capturing body tracking frame is Invalid!"));
		return;
	}

	// Capture a depth frame
	k4a_capture_t sensorCapture = NULL;
	k4a_wait_result_t getCaptureResult = k4a_device_get_capture(NativeKinectDevice, &sensorCapture, TimeOutInMilliSecs);
	if (getCaptureResult != K4A_WAIT_RESULT_SUCCEEDED)
	{
		UE_LOG(AzureKinectDeviceLog, Error, TEXT("Kinect device get capture %s!"),
			(getCaptureResult == K4A_WAIT_RESULT_FAILED ? TEXT("Failed") : TEXT("Timed Out")));
		return;
	}

	// Enqueue the capture
	k4a_wait_result_t queueCaptureResult = k4abt_tracker_enqueue_capture(NativeBodyTracker, sensorCapture, TimeOutInMilliSecs);
	// Release the sensor capture
	k4a_capture_release(sensorCapture);
	if (queueCaptureResult != K4A_WAIT_RESULT_SUCCEEDED)
	{
		//UE_LOG(AzureKinectDeviceLog, Error, TEXT("Adding capture to the Tracker process queue %s!"),
		//	(getCaptureResult == K4A_WAIT_RESULT_FAILED ? TEXT("Failed") : TEXT("Timed Out")));
		return;
	}

	k4abt_frame_t bodyFrame = NULL;
	k4a_wait_result_t popFrameResult = k4abt_tracker_pop_result(NativeBodyTracker, &bodyFrame, TimeOutInMilliSecs);
	if (popFrameResult != K4A_WAIT_RESULT_SUCCEEDED)
	{
		UE_LOG(AzureKinectDeviceLog, Error, TEXT("Tracker pop body frame result %s!"),
			(getCaptureResult == K4A_WAIT_RESULT_FAILED ? TEXT("Failed") : TEXT("Timed Out")));
		return;
	}

	// Successfully popped the body tracking result
	size_t numBodies = k4abt_frame_get_num_bodies(bodyFrame);
	//UE_LOG(AzureKinectDeviceLog, Warning, TEXT("%zu bodies are detected"), numBodies);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 5.0f, (numBodies > 0 ? FColor::Cyan : FColor::Red), FString::Printf(TEXT("%zu bodies are detected"), numBodies));
	}

	for (size_t i = 0; i < numBodies; i++)
	{
		k4abt_body_t body;
		k4a_result_t bodySkeletonResult = k4abt_frame_get_body_skeleton(bodyFrame, i, &body.skeleton);
		if (bodySkeletonResult != K4A_RESULT_SUCCEEDED)
		{
			UE_LOG(AzureKinectDeviceLog, Error, TEXT("Get Body Skeleton Failed!"));
			NativeBodies[i].bIsValid = false;
			continue;
		}

		body.id = k4abt_frame_get_body_id(bodyFrame, i);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Cyan, FString::Printf(TEXT("  Body Id : %d"), body.id));
		}

		// Cannot modify UObjects in thread
		//Bodies[i]->UpdateBodyWithKinectInfo(body);

		NativeBodies[i].NativeBody = body;
		NativeBodies[i].bIsValid = true;
	}

	// Release the body frame
	k4abt_frame_release(bodyFrame);
}

int32 AzureKinectDevice::GetTimeOutInMilliSecs() const
{
	return TimeOutInMilliSecs;
}

TArray<UAzureKinectBody*> AzureKinectDevice::GetBodies() const
{
	return Bodies;
}

bool AzureKinectDevice::IsInitialized() const
{
	return bIsInitialized;
}

void AzureKinectDevice::InitializeBodies()
{
	// reset any allocated memory
	Bodies.Empty(MaxBodies);
	Bodies.SetNum(MaxBodies);
	Bodies.Shrink();

	for (uint32 i = 0; i < MaxBodies; i++)
	{
		Bodies[i] = NewObject<UAzureKinectBody>();
		Bodies[i]->AddToRoot();
	}
}

void AzureKinectDevice::StartKinectThread()
{
	Thread = FAzureKinectThread::InitPolling(this);
}

bool AzureKinectDevice::OnTick(float DeltaTime)
{
	//UE_LOG(AzureKinectDeviceLog, Log, TEXT("Ticking : %f"), DeltaTime);
	if (bIsInitialized)
	{
		//for (AzureKinectBodyWrapper nativeBody : NativeBodies)
		for (uint32 i = 0; i < MaxBodies; i++)
		{
			//if (!NativeBodies[i].bIsValid) continue;

			Bodies[i]->UpdateBodyWithKinectInfo(NativeBodies[i].NativeBody, NativeBodies[i].bIsValid);
		}
	}

	return true;
}
