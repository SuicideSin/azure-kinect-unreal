// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <AzureKinectEnums.h>
#include <AzureKinectDevice.h>
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

	/**
	 * Initializes the azure kinect device with the given details and if the initialization is 
	 * successful, then adds it to the TMap.
	 *
	 * @param DeviceId The device id of the Azure Kinect Device to initialize
	 * @param DepthMode The default is set to NFOV_UNBINNED. For body tracking it should be NFOV_UNBINNED or WFOV_BINNED
	 * @param TimeOutInMilliSecs Default is Zero (Non-blocking). Set it to -1 (K4A_WAIT_INFINITE) for Blocking call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Azure Kinect", meta = (DisplayName = "Init Azure Kinect"))
	static void InitDevice(int32 DeviceId = 0, EKinectDepthMode DepthMode = EKinectDepthMode::K4A_DEPTH_MODE_NFOV_UNBINNED, int32 TimeOutInMilliSecs = 0);

	UFUNCTION(BlueprintCallable, Category = "Azure Kinect", meta = (DisplayName = "Shutdown Azure Kinect"))
	static void ShutdownDevice(int32 DeviceId = 0);

	UFUNCTION(BlueprintPure, Category = "Azure Kinect", meta = (DisplayName = "Is Azure Kinect Device Initialized"))
	static bool IsDeviceInitialized(int32 DeviceId = 0);

	//UFUNCTION(BlueprintCallable, Category = "Azure Kinect", meta = (DisplayName = "Get Azure Kinect Device"))
	static AzureKinectDevice* GetDevice(int32 DeviceId);

	UFUNCTION(BlueprintPure, Category = "Azure Kinect", meta = (DisplayName = "Get Bodies for Device"))
	static TArray<UAzureKinectBody*> GetBodies(int32 DeviceId);

	UFUNCTION(BlueprintPure, Category = "Azure Kinect", meta = (DisplayName = "Get Kinect Body Joint Count"))
	static int32 GetJointCount();

private:
	static UAzureKinectManager *Instance;
	
	TMap<int32, AzureKinectDevice*> KinectDevicesById;

	void ShutdownAllDevices();
};
