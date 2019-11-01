#pragma once

#include "AzureKinectEnums.generated.h"

/**
 * Blueprintable enum defined based on k4a_depth_mode_t from k4atypes.h
 *
 * @remarks This should always have the same enum values as k4a_depth_mode_t
 */
UENUM(BlueprintType, Category = "Azure Kinect|Enums")
enum class EKinectDepthMode : uint8
{
	K4A_DEPTH_MODE_OFF = 0 UMETA(DisplayName = "Depth Mode Off"),        /**< Depth sensor will be turned off with this setting. */
	K4A_DEPTH_MODE_NFOV_2X2BINNED UMETA(DisplayName = "NFOV 2x2 Binned (320x288)"), /**< Depth captured at 320x288. Passive IR is also captured at 320x288. */
	K4A_DEPTH_MODE_NFOV_UNBINNED UMETA(DisplayName = "NFOV Unbinned (640x576)"),  /**< Depth captured at 640x576. Passive IR is also captured at 640x576. */
	K4A_DEPTH_MODE_WFOV_2X2BINNED UMETA(DisplayName = "WFOV 2x2 Binned (512x512)"), /**< Depth captured at 512x512. Passive IR is also captured at 512x512. */
	K4A_DEPTH_MODE_WFOV_UNBINNED UMETA(DisplayName = "WFOV Unbinned (1024x1024)"),  /**< Depth captured at 1024x1024. Passive IR is also captured at 1024x1024. */
	K4A_DEPTH_MODE_PASSIVE_IR UMETA(DisplayName = "Passive IR (1024x1024)"),     /**< Passive IR only, captured at 1024x1024. */
};

/**
 * Blueprintable enum defined based on k4abt_joint_id_t from k4abttypes.h
 *
 * @remarks This should always have the same enum values as k4abt_joint_id_t
 */
UENUM(BlueprintType, Category = "Azure Kinect|Enums")
enum class EKinectBodyJointId : uint8
{
	K4ABT_JOINT_PELVIS = 0 UMETA(DisplayName = "Pelvis"),
	K4ABT_JOINT_SPINE_NAVAL UMETA(DisplayName = "Spine Naval"),
	K4ABT_JOINT_SPINE_CHEST UMETA(DisplayName = "Spine Chest"),
	K4ABT_JOINT_NECK UMETA(DisplayName = "Neck"),
	K4ABT_JOINT_CLAVICLE_LEFT UMETA(DisplayName = "Clavicle Left"),
	K4ABT_JOINT_SHOULDER_LEFT UMETA(DisplayName = "Shoulder Left"),
	K4ABT_JOINT_ELBOW_LEFT UMETA(DisplayName = "Elbow Left"),
	K4ABT_JOINT_WRIST_LEFT UMETA(DisplayName = "Wrist Left"),
	K4ABT_JOINT_CLAVICLE_RIGHT UMETA(DisplayName = "Clavicle Right"),
	K4ABT_JOINT_SHOULDER_RIGHT UMETA(DisplayName = "Shoulder Right"),
	K4ABT_JOINT_ELBOW_RIGHT UMETA(DisplayName = "Elbow Right"),
	K4ABT_JOINT_WRIST_RIGHT UMETA(DisplayName = "Wrist Right"),
	K4ABT_JOINT_HIP_LEFT UMETA(DisplayName = "Hip Left"),
	K4ABT_JOINT_KNEE_LEFT UMETA(DisplayName = "Knee Left"),
	K4ABT_JOINT_ANKLE_LEFT UMETA(DisplayName = "Ankle Left"),
	K4ABT_JOINT_FOOT_LEFT UMETA(DisplayName = "Foot Left"),
	K4ABT_JOINT_HIP_RIGHT UMETA(DisplayName = "Hip Right"),
	K4ABT_JOINT_KNEE_RIGHT UMETA(DisplayName = "Knee Right"),
	K4ABT_JOINT_ANKLE_RIGHT UMETA(DisplayName = "Ankle Right"),
	K4ABT_JOINT_FOOT_RIGHT UMETA(DisplayName = "Foot Right"),
	K4ABT_JOINT_HEAD UMETA(DisplayName = "Head"),
	K4ABT_JOINT_NOSE UMETA(DisplayName = "Nose"),
	K4ABT_JOINT_EYE_LEFT UMETA(DisplayName = "Eye Left"),
	K4ABT_JOINT_EAR_LEFT UMETA(DisplayName = "Ear Left"),
	K4ABT_JOINT_EYE_RIGHT UMETA(DisplayName = "Eye Right"),
	K4ABT_JOINT_EAR_RIGHT UMETA(DisplayName = "Ear Right"),
	K4ABT_JOINT_COUNT UMETA(DisplayName = "COUNT"),
};
