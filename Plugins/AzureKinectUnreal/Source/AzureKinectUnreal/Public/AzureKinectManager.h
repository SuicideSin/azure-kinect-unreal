// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
#include <k4a/k4a.h>
#include <k4abt.h>
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

	static void InitDevice(uint32 DeviceId = 0, k4a_depth_mode_t DepthMode = K4A_DEPTH_MODE_NFOV_UNBINNED);

	static void CaptureBodyTrackingFrame(k4a_device_t Device, int32 TimeOutInMilliSecs = 0);

	//static k4a_device_t GetDevice(uint32 DeviceId);

private:
	static k4a_device_t KinectDevice;

	static k4abt_tracker_t BodyTracker;
};
