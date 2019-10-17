// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AzureKinectUnreal.h"
#include "IPluginManager.h"
#include "PlatformProcess.h"
#include "Paths.h"
#include "MessageDialog.h"

#define LOCTEXT_NAMESPACE "FAzureKinectUnrealModule"

void* FAzureKinectUnrealModule::K4ADllHandle = nullptr;
void* FAzureKinectUnrealModule::K4ABTDllHandle = nullptr;

void FAzureKinectUnrealModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	const FString baseDir = IPluginManager::Get().FindPlugin("AzureKinectUnreal")->GetBaseDir();
	const FString dllPath = FPaths::Combine(*baseDir, TEXT("Source"), TEXT("ThirdParty"), TEXT("dlls"));
	UE_LOG(LogTemp, Warning, TEXT("Dll Path : %s"), *dllPath);
	FString k4aDll = "k4a.dll";	// +FPlatformProcess::GetModuleExtension();
	FString k4abtDll = "k4abt.dll";
	
	K4ADllHandle = FPlatformProcess::GetDllHandle(*FPaths::Combine(*dllPath, *k4aDll));
	K4ABTDllHandle = FPlatformProcess::GetDllHandle(*FPaths::Combine(*dllPath, *k4abtDll));

	if (K4ADllHandle)
	{
		StartInternal();
	}
}

void FAzureKinectUnrealModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (K4ADllHandle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(K4ADllHandle);
		K4ADllHandle = nullptr;
	}

	if (K4ABTDllHandle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(K4ABTDllHandle);
		K4ABTDllHandle = nullptr;
	}
}

void FAzureKinectUnrealModule::StartInternal()
{
	k4a_device_t KinectDevice = NULL;
	k4a_result_t deviceOpenResult = k4a_device_open(0, &KinectDevice);
	if (deviceOpenResult != K4A_RESULT_SUCCEEDED)
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Error", "Open Kinect device Failed!"));
		return;
	}

	// Start the Camera and make sure the Depth Camera is Enabled
	k4a_device_configuration_t deviceConfig = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	deviceConfig.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
	deviceConfig.color_resolution = K4A_COLOR_RESOLUTION_OFF;

	k4a_result_t deviceStartCameraResult = k4a_device_start_cameras(KinectDevice, &deviceConfig);
	if (deviceStartCameraResult != K4A_RESULT_SUCCEEDED)
	{
		ShutdownInternal(KinectDevice, NULL);
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Error", "Kinect device start camera Failed!"));
		return;
	}

	k4a_calibration_t sensorCalibration;
	k4a_result_t deviceCalibrationResult = k4a_device_get_calibration(KinectDevice, deviceConfig.depth_mode, deviceConfig.color_resolution, &sensorCalibration);
	if (deviceCalibrationResult != K4A_RESULT_SUCCEEDED)
	{
		ShutdownInternal(KinectDevice, NULL);
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Error", "Kinect device get depth camera calibration Failed!"));
		return;
	}

	if (K4ABTDllHandle)
	{
		k4abt_tracker_t BodyTracker = NULL;
		k4abt_tracker_configuration_t trackerConfig = K4ABT_TRACKER_CONFIG_DEFAULT;
		k4a_result_t trackerCreateResult = k4abt_tracker_create(&sensorCalibration, trackerConfig, &BodyTracker);
		if (trackerCreateResult != K4A_RESULT_SUCCEEDED)
		{
			ShutdownInternal(KinectDevice, BodyTracker);
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Error", "Kinect device - Body Tracker creation Failed!"));
			return;
		}

		ShutdownInternal(KinectDevice, BodyTracker);
	}

	ShutdownInternal(KinectDevice, NULL);
}

void FAzureKinectUnrealModule::ShutdownInternal(k4a_device_t Device, k4abt_tracker_t Tracker)
{
	if (Tracker)
	{
		k4abt_tracker_shutdown(Tracker);
		k4abt_tracker_destroy(Tracker);
	}

	if (Device)
	{
		k4a_device_stop_cameras(Device);
		k4a_device_close(Device);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAzureKinectUnrealModule, AzureKinectUnreal)
