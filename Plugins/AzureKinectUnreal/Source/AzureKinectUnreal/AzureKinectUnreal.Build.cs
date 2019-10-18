// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class AzureKinectUnreal : ModuleRules
{
	public AzureKinectUnreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				Path.Combine(ModuleDirectory, "Public")
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				Path.Combine(ModuleDirectory, "Private")
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		// Include Azure Kinect Sensor and Body Tracking SDKs
		string thirdPartyPath = Path.Combine(ModuleDirectory, "../../ThirdParty");  //ModuleDirectory = Project/Plugins/AzureKinectUnreal/Source/AzureKinectUnreal
		string azureKinectSensorSDKPath = Path.Combine(thirdPartyPath, "AzureKinectSensor");
		string azureKinectBodyTrackingSDKPath = Path.Combine(thirdPartyPath, "AzureKinectBodyTracking");

		PublicIncludePaths.AddRange(
			new string[]
			{
				Path.Combine(azureKinectSensorSDKPath, "include"),
				Path.Combine(azureKinectBodyTrackingSDKPath, "include")
			});

		// Libs path
		PublicLibraryPaths.Add(Path.Combine(azureKinectSensorSDKPath, "amd64", "lib"));
		PublicLibraryPaths.Add(Path.Combine(azureKinectBodyTrackingSDKPath, "amd64", "lib"));

		PublicAdditionalLibraries.AddRange(
			new string[]
			{
				"k4a.lib",
				"k4abt.lib"
			});
	}
}
