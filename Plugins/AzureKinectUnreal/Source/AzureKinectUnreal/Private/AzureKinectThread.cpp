// Fill out your copyright notice in the Description page of Project Settings.


#include "AzureKinectThread.h"
#include "HAL/PlatformProcess.h"

DEFINE_LOG_CATEGORY(AzureKinectThreadLog);

FAzureKinectThread *FAzureKinectThread::Instance = nullptr;

FAzureKinectThread::FAzureKinectThread() :
	KinectThread(nullptr),
	StopThreadCounter(0)
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

FAzureKinectThread * FAzureKinectThread::InitPolling()
{
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Instance && FPlatformProcess::SupportsMultithreading())
	{
		Instance = new FAzureKinectThread();
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
	UE_LOG(AzureKinectThreadLog, Log, TEXT("Azure Kinect thread running."));

	while (StopThreadCounter.GetValue() == 0)
	{

	}

	return 0;
}

void FAzureKinectThread::Stop()
{
	StopThreadCounter.Increment();
}
