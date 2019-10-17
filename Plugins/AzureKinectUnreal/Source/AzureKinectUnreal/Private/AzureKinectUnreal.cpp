// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AzureKinectUnreal.h"
#include "IPluginManager.h"
#include "PlatformProcess.h"
#include "Paths.h"

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

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAzureKinectUnrealModule, AzureKinectUnreal)
