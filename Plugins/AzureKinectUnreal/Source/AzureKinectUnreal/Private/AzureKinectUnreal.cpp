// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AzureKinectUnreal.h"
#include "Interfaces/IPluginManager.h"
#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FAzureKinectUnrealModule"

void FAzureKinectUnrealModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	const FString baseDir = IPluginManager::Get().FindPlugin("AzureKinectUnreal")->GetBaseDir();
	const FString dllPath = FPaths::Combine(*baseDir, TEXT("ThirdParty"), TEXT("dlls"));
	UE_LOG(LogTemp, Warning, TEXT("Dll Path : %s"), *dllPath);
	FString k4aDll = "k4a.dll";

	K4ADllHandle = FPlatformProcess::GetDllHandle(*FPaths::Combine(*dllPath, *k4aDll));
	K4ABTDllHandle = FPlatformProcess::GetDllHandle(*FPaths::Combine(*dllPath, TEXT("k4abt.dll")));
}

void FAzureKinectUnrealModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (K4ADllHandle)
	{
		FPlatformProcess::FreeDllHandle(K4ADllHandle);
		K4ADllHandle = nullptr;
	}

	if (K4ABTDllHandle)
	{
		FPlatformProcess::FreeDllHandle(K4ABTDllHandle);
		K4ABTDllHandle = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAzureKinectUnrealModule, AzureKinectUnreal)
