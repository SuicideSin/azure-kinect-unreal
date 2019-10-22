// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
#include <k4a/k4a.h>
#include <k4abt.h>
#include <AzureKinectEnums.h>
#include "AzureKinectManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(AzureKinectLog, Log, All);

/**
 * 
 */
UCLASS(BlueprintType, Category = "Azure Kinect")
class AZUREKINECTUNREAL_API UAzureKinectManager : public UObject
{
	GENERATED_BODY()
	
public:
	/** Ctor */
	UAzureKinectManager();
	/** Destructor */
	~UAzureKinectManager();

	UFUNCTION(BlueprintCallable, Category = "Azure Kinect", meta = (DisplayName = "Init Azure Kinect"))
	static void InitDevice(int32 DeviceId = 0, EKinectDepthMode DepthMode = EKinectDepthMode::K4A_DEPTH_MODE_NFOV_UNBINNED);

	/** 
	 * Captures the device body tracking frame, enqueues the capture, pops the tracker body frame result 
	 * and releases the body frame.
	 *
	 * @param TimeOutInMilliSecs Default is Zero (Non-blocking). Set it to -1 (K4A_WAIT_INFINITE) for Blocking call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Azure Kinect", meta = (DisplayName = "Capture Body Tracking Frame"))
	static void CaptureBodyTrackingFrame(int32 TimeOutInMilliSecs = 0);	//k4a_device_t Device,

	UFUNCTION(BlueprintCallable, Category = "Azure Kinect", meta = (DisplayName = "Shutdown Azure Kinect"))
	static void ShutdownDevice(int32 DeviceId = 0);

	//UFUNCTION(BlueprintCallable, Category = "Azure Kinect", meta = (DisplayName = "Get Azure Kinect Device"))
	//static k4a_device_t GetDevice(uint32 DeviceId);

private:
	static k4a_device_t KinectDevice;

	static k4abt_tracker_t BodyTracker;
};
