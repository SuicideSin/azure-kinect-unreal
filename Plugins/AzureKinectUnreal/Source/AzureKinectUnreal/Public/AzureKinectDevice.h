// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "k4a/k4a.h"
#include "AzureKinectThread.h"
#include "AzureKinectBody.h"

DECLARE_LOG_CATEGORY_EXTERN(AzureKinectDeviceLog, Log, All);

/**
 * 
 */
class AZUREKINECTUNREAL_API AzureKinectDevice
{
public:
	AzureKinectDevice();
	AzureKinectDevice(int32 DeviceId, int32 TimeOut);
	~AzureKinectDevice();

	bool Initialize(k4a_depth_mode_t DepthMode);
	void Shutdown();

	/**
	 * Captures the device body tracking frame, enqueues the capture, pops the tracker body frame result
	 * and releases the body frame.
	 */
	void CaptureBodyTrackingFrame();

	int32 GetTimeOutInMilliSecs() const;

	TArray<UAzureKinectBody*> GetBodies() const;

private:
	k4a_device_t NativeKinectDevice;
	int32 DeviceId;
	//k4a_calibration_t SensorCalibration;

	k4abt_tracker_t NativeBodyTracker;
	int32 TimeOutInMilliSecs;

	FAzureKinectThread *Thread;

	bool bIsInitialized;

	TArray<UAzureKinectBody*> Bodies;

	// TODO : Find out how many bodies can be tracked with one Sensor
	const uint32 MaxBodies = 6;

	AzureKinectBodyWrapper NativeBodies[6];

	void InitializeBodies();

	void StartKinectThread();
};
