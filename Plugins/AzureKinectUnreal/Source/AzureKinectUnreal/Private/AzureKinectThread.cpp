// Fill out your copyright notice in the Description page of Project Settings.


#include "AzureKinectThread.h"
#include "HAL/PlatformProcess.h"
#include "AzureKinectDevice.h"

DEFINE_LOG_CATEGORY(AzureKinectThreadLog);

FAzureKinectThread *FAzureKinectThread::Instance = nullptr;

FAzureKinectThread::FAzureKinectThread(AzureKinectDevice *Device) :
	KinectThread(nullptr),
	StopThreadCounter(0),
	KinectDevice(Device)
{
	KinectThread = FRunnableThread::Create(this, TEXT("FAzureKinectThread"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
	if (!KinectThread)
	{
		UE_LOG(AzureKinectThreadLog, Error, TEXT("Failed to create Azure Kinect thread."));
	}
}

FAzureKinectThread::~FAzureKinectThread()
{
	if (KinectThread)
	{
		delete KinectThread;
		KinectThread = nullptr;
	}
}

FAzureKinectThread * FAzureKinectThread::InitPolling(AzureKinectDevice *Device)
{
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Instance && FPlatformProcess::SupportsMultithreading())
	{
		Instance = new FAzureKinectThread(Device);
	}
	return Instance;
}

void FAzureKinectThread::Shutdown()
{
	if (Instance)
	{
		Instance->EnsureCompletion();
		delete Instance;
		Instance = nullptr;
	}
}

//bool FAzureKinectThread::IsThreadFinished()
//{
//	if (Instance) return Instance->IsFinished();
//	return true;
//}

void FAzureKinectThread::EnsureCompletion()
{
	Stop();
	if (KinectThread)
	{
		KinectThread->WaitForCompletion();
	}
}

bool FAzureKinectThread::Init()
{
	UE_LOG(AzureKinectThreadLog, Log, TEXT("Azure Kinect thread started."));
	return false;
}

uint32 FAzureKinectThread::Run()
{
	if (!KinectDevice)
	{
		UE_LOG(AzureKinectThreadLog, Error, TEXT("KinectDevice is null, could not run the thread"));
		return 1;
	}

	const float UpdateInterval = FMath::Max(0.0f, (KinectDevice->GetTimeOutInMilliSecs() / 1000.0f));
	UE_LOG(AzureKinectThreadLog, Log, TEXT("Azure Kinect thread running with interval : %d"), UpdateInterval);

	while (StopThreadCounter.GetValue() == 0)
	{
		// Do Kinect capture, enqueue, pop body frame stuff
		KinectDevice->CaptureBodyTrackingFrame();

		// may be don't need this since the Kinect API calls will
		// be blocking if the Timeout is non-zero.
		//if (UpdateInterval > 0.0f)
		//{
		//	FPlatformProcess::Sleep(UpdateInterval);
		//}
	}

	return 0;
}

void FAzureKinectThread::Stop()
{
	StopThreadCounter.Increment();
}
