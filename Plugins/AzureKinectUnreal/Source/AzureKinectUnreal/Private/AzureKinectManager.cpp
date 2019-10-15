// Fill out your copyright notice in the Description page of Project Settings.


#include "AzureKinectManager.h"

DEFINE_LOG_CATEGORY(AzureKinectLog);

k4a_device_t UAzureKinectManager::KinectDevice = NULL;
k4abt_tracker_t UAzureKinectManager::BodyTracker = NULL;

UAzureKinectManager::UAzureKinectManager()
{
}

UAzureKinectManager::~UAzureKinectManager()
{
	if (BodyTracker)
	{
		k4abt_tracker_shutdown(BodyTracker);
		k4abt_tracker_destroy(BodyTracker);
	}

	if (KinectDevice)
	{
		k4a_device_stop_cameras(KinectDevice);
		k4a_device_close(KinectDevice);
	}
}

void UAzureKinectManager::InitDevice(uint32 DeviceId, k4a_depth_mode_t DepthMode)
{
	// Open the Azure Kinect Device
	k4a_result_t deviceOpenResult = k4a_device_open(DeviceId, &KinectDevice);
	if (deviceOpenResult != K4A_RESULT_SUCCEEDED)
	{
		UE_LOG(AzureKinectLog, Error, TEXT("Open Kinect device (id : %d) Failed!"), DeviceId);
		return;
	}

	// Start the Camera and make sure the Depth Camera is Enabled
	k4a_device_configuration_t deviceConfig = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	deviceConfig.depth_mode = DepthMode;
	deviceConfig.color_resolution = K4A_COLOR_RESOLUTION_OFF;

	k4a_result_t deviceStartCameraResult = k4a_device_start_cameras(KinectDevice, &deviceConfig);
	if (deviceStartCameraResult != K4A_RESULT_SUCCEEDED)
	{
		UE_LOG(AzureKinectLog, Error, TEXT("Kinect device (id : %d) start camera Failed!"), DeviceId);
		return;
	}

	k4a_calibration_t sensorCalibration;
	k4a_result_t deviceCalibrationResult = k4a_device_get_calibration(KinectDevice, deviceConfig.depth_mode, deviceConfig.color_resolution, &sensorCalibration);
	if (deviceCalibrationResult != K4A_RESULT_SUCCEEDED)
	{
		UE_LOG(AzureKinectLog, Error, TEXT("Kinect device (id : %d) get depth camera calibration Failed!"), DeviceId);
		return;
	}

	//k4abt_tracker_t tracker = NULL;
	k4abt_tracker_configuration_t trackerConfig = K4ABT_TRACKER_CONFIG_DEFAULT;
	k4a_result_t trackerCreateResult = k4abt_tracker_create(&sensorCalibration, trackerConfig, &BodyTracker);
	if (trackerCreateResult != K4A_RESULT_SUCCEEDED)
	{
		UE_LOG(AzureKinectLog, Error, TEXT("Kinect device (id : %d) - Body Tracker creation Failed!"), DeviceId);
		return;
	}
}

//k4a_device_t UAzureKinectManager::GetDevice(uint32 DeviceId)
//{
//	return Device;
//}
