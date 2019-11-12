# Azure Kinect Unreal Plugin

This Unreal project contains the Azure Kinect Plugin and sample map to test the Plugin.

### SDKs

- [Azure Kinect SDK 1.2.0](https://docs.microsoft.com/en-us/azure/kinect-dk/sensor-sdk-download)
- [Azure Kinect Body Tracking SDK 0.9.3](https://docs.microsoft.com/en-us/azure/kinect-dk/body-sdk-download)

## Plugin Files

 - [AzureKinectManager](Plugins/AzureKinectUnreal/Source/AzureKinectUnreal/Public/AzureKinectManager.h) : A singleton class used in Blueprints to control and get info from the connected Azure Kinect Device(s).
 - [AzureKinectDevice](Plugins/AzureKinectUnreal/Source/AzureKinectUnreal/Public/AzureKinectDevice.h) : A representation of the Native Azure Kinect Device that is used to call the connected device's (K4A & K4ABT) API for starting & stopping camera sensors, creating & destroying the body tracker, capturing the body frame etc.
 - [AzureKinectThread](Plugins/AzureKinectUnreal/Source/AzureKinectUnreal/Public/AzureKinectThread.h) : A thread that is used to capture the Azure Kinect device's body frame data.
 - [AzureKinectBody](Plugins/AzureKinectUnreal/Source/AzureKinectUnreal/Public/AzureKinectBody.h) : A representation of the Native Azure Kinect Body that contains Joints.
	- [AzureKinectJoint](https://github.com/secretlocation/azure-kinect-unreal/blob/001c05ae5a941518120e3719cb4f62eaff9a5b84/Plugins/AzureKinectUnreal/Source/AzureKinectUnreal/Public/AzureKinectBody.h#L18) : A representation of the Native Azure Kinect Body Skeleton joint. This stores the kinect skeleton joint position and orientation data by converting them from Kinect camera co-ordinate system to Unreal co-ordinate system and maps them to the Unreal mannequin.
 - [AzureKinectHelper](Plugins/AzureKinectUnreal/Source/AzureKinectUnreal/Public/AzureKinectHelper.h) : A helper file that contains the duplicated Azure Kinect enums for Blueprint access and constant values for maximum bodies tracked and the joints count. 

### Sample Files for Testing

Blueprints/
 - BP_AnimMannequin : The animation blueprint that applies the Orientation of the joints from the plugin to the UE4 mannequin.
 - BP_MannequinActor : A test blueprint actor with 1 UE4 mannequin skeletal mesh that uses the above animation blueprint.
 - BP_TenMannequinActors : A test blueprint actor with 10 UE4 mannequin skeletal meshes that use the above animation blueprint.
 - BP_TestActor : A test blueprint actor with 26 spheres where each one represents a joint in Azure kinect body skeleton.The spheres use the Position data from the plugin for their corresponding joints that they each represent.

Maps/
 - Sample : This map contains the BP_TestActor and BP_MannequinActor already placed in the level for testing.

## Notes

The following dlls and their dependency files are needed for the Azure Kinect API to work.
Since Unreal will search in `Engine/Binaries/Win64`, `Project/Binaries/Win64`, `Engine/Plugins/.../Binaries/Win64` and `Project/Plugins/.../Binaries/Win64` folders to load the dependent dll files, the following are put in the plugin binaries folder. This will ensure that the project will compile and open in UE4 editor.
```
Plugins/AzureKinectUnreal/Binaries/Win64/
 - k4a.dll
 - k4abt.dll
 - depthengine_2_0.dll
```
_**Important Note:** In order to run the project in Unreal Editor and call the Azure Kinect API without issues, the below mentioned dependent files should be copied to the Unreal installation folder where `UE4Editor.exe` resides._
_Another thing that can be done instead of copying the below dll files to `Unreal installation` folder is to add the folder path where these dlls reside to the `Path` variable in `Environment Variables` for User or System. The exception is `dnn_model_2_0.onnx` file which **should** be copied to the Unreal installation folder in order for `k4abt (Body Tracking)` API to work._
```
<unreal_installation_folder>/Engine/Binaries/Win64/
 - onnxruntime.dll
 - dnn_model_2_0.onnx
 - cudnn64_7.dll
 - cudart64_100.dll
 - cublas64_100.dll
```
Reference : [Azure Kinect SDK - Issue 709](https://github.com/microsoft/Azure-Kinect-Sensor-SDK/issues/709#issuecomment-545121230)
