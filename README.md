# Azure Kinect Unreal Plugin

This Unreal project contains the Azure Kinect Plugin and sample map to test the Plugin.

### SDKs

- [Azure Kinect SDK 1.2.0](https://docs.microsoft.com/en-us/azure/kinect-dk/sensor-sdk-download)
- [Azure Kinect Body Tracking SDK 0.9.3](https://docs.microsoft.com/en-us/azure/kinect-dk/body-sdk-download)

## Notes

The following dlls and their dependency files are needed for the Azure Kinect API to work.
 - k4a.dll
 - k4abt.dll
 - depthengine_2_0.dll
 - onnxruntime.dll
 - dnn_model_2_0.onnx
 - cudnn64_7.dll
 - cudart64_100.dll
 - cublas64_100.dll

Currently, they are put in Plugin Binaries folder (`Plugins/AzureKinectUnreal/Binaries/Win64/`) since Unreal will search in Engine/Binaries/Win64, Project/Binaries/Win64, Engine/Plugins/.../Binaries/Win64 and Project/Plugins/.../Binaries/Win64 and load them.

_**Important Note:** In order to run the project in Unreal Editor and call the Azure Kinect API without issues, the above mentioned dependent files should be copied to the Unreal installation folder where `UE4Editor.exe` resides._

If the dlls are put in a **different folder** then the dll will not be loaded and Unreal will most likely crash when the call to the Kinect API is made.
In order to fix this, the following should be done.
 1. The dll path should be added to `PublicLibraryPaths` and the dll should be added to `PublicDelayLoadDLLs` in the `<Plugin>.Build.cs`.
    
	Ex:
	#### AzureKinectUnreal.Build.cs
    ```
    // Dlls path = Project/Plugins/AzureKinectUnreal/ThirdParty/dlls
    // ModuleDirectory = Project/Plugins/AzureKinectUnreal/Source/AzureKinectUnreal
    PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "../../ThirdParty", "dlls"));
    
    PublicDelayLoadDLLs.AddRange(
    	new string[]
    	{
    		"k4a.dll",
    		"k4abt.dll"
    	});
    ```
   
 2. The dlls should be loaded in `StartupModule` and released in `ShutdownModule`.
    
	Ex:
    #### AzureKinectUnreal.h
    ```
    private:
    	void* K4ADllHandle = nullptr;
    	void* K4ABTDllHandle = nullptr;
    ```
    
    #### AzureKinectUnreal.cpp
    ```
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
    ```
