// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "k4a/k4a.h"
#include "k4abt.h"

class FAzureKinectUnrealModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	static void* K4ADllHandle;
	static void* K4ABTDllHandle;

	void StartInternal();
	void ShutdownInternal(k4a_device_t Device, k4abt_tracker_t Tracker);
};
