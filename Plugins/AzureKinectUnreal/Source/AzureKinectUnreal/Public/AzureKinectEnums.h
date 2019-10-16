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
