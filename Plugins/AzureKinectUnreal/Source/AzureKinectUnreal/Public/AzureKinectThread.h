// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

DECLARE_LOG_CATEGORY_EXTERN(AzureKinectThreadLog, Log, All);

/**
 * 
 */
class AZUREKINECTUNREAL_API FAzureKinectThread : public FRunnable
{
public:
	FAzureKinectThread();
	~FAzureKinectThread();

	static FAzureKinectThread *InitPolling();

	static void Shutdown();

	//static bool IsThreadFinished();
	
	void EnsureCompletion();

	// Begin FRunnable interface
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End FRunnable interface

private:
	static FAzureKinectThread *Instance;

	FRunnableThread *KinectThread;

	FThreadSafeCounter StopThreadCounter;


};
