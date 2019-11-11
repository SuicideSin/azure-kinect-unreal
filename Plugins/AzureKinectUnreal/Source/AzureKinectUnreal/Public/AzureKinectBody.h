// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "k4abt.h"
#include "AzureKinectEnums.h"
#include "AzureKinectBody.generated.h"

/**
 * A representation of the Azure Kinect Body Skeleton joint.
 * This stores the kinect skeleton joint position and orientation 
 * data by converting them from Kinect camera co-ordinate system to
 * Unreal co-ordinate system and maps them to the Unreal mannequin.
 */
USTRUCT(BlueprintType, Category = "Azure Kinect|Body|Joints")
struct FAzureKinectJoint
{
	GENERATED_BODY()

	/** The position of the joint to be mapped to Unreal mannequin. */
	UPROPERTY(BlueprintReadOnly, Category = "Azure Kinect|Body|Joints")
	FVector Position;

	/** The rotation of the joint to be mapped to Unreal mannequin. */
	UPROPERTY(BlueprintReadOnly, Category = "Azure Kinect|Body|Joints")
	FRotator Orientation;

	//UPROPERTY(BlueprintReadWrite, Category = "Azure Kinect|Body|Joints")
	float PositionMultiplier = 0.5f;

	/** The index of the joint that corresponds to the integral value of the joint enum. */
	UPROPERTY(BlueprintReadOnly, Category = "Azure Kinect|Body|Joints")
	int32 Index;

	/** Ctor */
	FAzureKinectJoint() :
		Position(FVector::ZeroVector),
		Orientation(FRotator::ZeroRotator),
		Index(0)
	{
	}

	/** Update the joint's position and rotation from the kinect joint data. */
	void UpdateFromNativeJoint(const k4abt_joint_t &NativeJoint, bool IsMirrored = false)
	{
		int32 XMultiplier = 1;	// (IsMirrored ? 1 : -1);
		/** 
		 * Convert Azure Kinect Depth and Color camera co-ordinate system
		 * to Unreal co-ordinate system
		 * @see https://docs.microsoft.com/en-us/azure/kinect-dk/coordinate-systems
		 * 
		 * Kinect Camera				Unreal
		 * --------------------------------------
		 * +ve X-axis		Right		+ve Y-axis
		 * +ve Y-axis		Down		-ve Z-axis
		 * +ve Z-axis		Forward		+ve X-axis
		 */
		Position.Set(NativeJoint.position.xyz.z * PositionMultiplier * XMultiplier, NativeJoint.position.xyz.x * PositionMultiplier, -NativeJoint.position.xyz.y * PositionMultiplier);

		/**
		 * Convert the Orientation from Kinect co-ordinate system to Unreal co-ordinate system.
		 * We negate the x, y components of the JointQuaternion since we are converting from 
		 * Kinect's Right Hand orientation to Unreal's Left Hand orientation.
		 */
		FQuat JointQuaternion = FQuat(
			-NativeJoint.orientation.wxyz.z,
			-NativeJoint.orientation.wxyz.x,
			NativeJoint.orientation.wxyz.y,
			NativeJoint.orientation.wxyz.w);

		/**
		 * Map the Azure Kinect joint orientation to Unreal Mannequin.
		 * @see https://docs.microsoft.com/en-us/azure/kinect-dk/body-joints
		 * 
		 */
#define JOINT_ID(Name) static_cast<uint8>(EKinectBodyJointId::K4ABT_JOINT_##Name)

		// 
		if (Index <= JOINT_ID(NECK) 
			|| (Index >= JOINT_ID(HEAD) && Index <= JOINT_ID(EAR_RIGHT))
			|| (Index >= JOINT_ID(HIP_LEFT) && Index <= JOINT_ID(FOOT_LEFT))
			|| (Index >= JOINT_ID(HIP_RIGHT) && Index <= JOINT_ID(FOOT_RIGHT)))
		{
			Orientation = FRotationMatrix::MakeFromXZ(JointQuaternion.GetAxisY(), -JointQuaternion.GetAxisX())
				.Rotator();

			//Orientation = JointQuaternion.Rotator();
		}
		else if (Index == JOINT_ID(SHOULDER_LEFT) || Index == JOINT_ID(SHOULDER_RIGHT))
		{
			Orientation = FRotationMatrix::MakeFromXZ(JointQuaternion.GetAxisY(), JointQuaternion.GetAxisZ())
				.Rotator();
		}
		else if (Index == JOINT_ID(ELBOW_LEFT) || Index == JOINT_ID(WRIST_LEFT)
			|| Index == JOINT_ID(ELBOW_RIGHT) || Index == JOINT_ID(WRIST_RIGHT))
		{
			Orientation = FRotationMatrix::MakeFromXZ(JointQuaternion.GetAxisY(), -JointQuaternion.GetAxisZ())
				.Rotator();
		}

#undef JOINT_ID
	}
};

/** 
 * A wrapper to the k4abt body
 */
struct AzureKinectBodyWrapper
{
	k4abt_body_t NativeBody;

	bool bIsValid;
	
	AzureKinectBodyWrapper() :
		bIsValid(false)
	{
	}
};

/**
 * A representation of the Azure Kinect Body that contains Joints.
 */
UCLASS(BlueprintType, Category = "Azure Kinect|Body")
class AZUREKINECTUNREAL_API UAzureKinectBody : public UObject
{
	GENERATED_BODY()
	
public:
	/** Ctor */
	UAzureKinectBody();
	/** Destructor */
	~UAzureKinectBody();

	/** Returns the Id of the Body. */
	UFUNCTION(BlueprintPure, Category = "Azure Kinect|Body")
	int32 GetId() const;

	/** Returns an array of Joints. */
	UFUNCTION(BlueprintPure, Category = "Azure Kinect|Body|Joints")
	TArray<FAzureKinectJoint> GetJoints() const;

	/** Returns a joint with the specified index. */
	UFUNCTION(BlueprintPure, Category = "Azure Kinect|Body|Joints")
	FAzureKinectJoint GetJoint(int32 index) const;

	/**
	 * Updates this body with the Kinect body data.
	 * 
	 * @param NativeBody A reference to the k4abt body handle.
	 * @param IsTracked Whether this body is currently being tracked or not.
	 */
	void UpdateBodyWithKinectInfo(const k4abt_body_t &NativeBody, bool IsTracked);

	/** Whether to mirror the joints. */
	UPROPERTY(BlueprintReadWrite, Category = "Azure Kinect|Body")
	bool bIsMirrored;

	/** Whether this body is tracked or not. */
	UPROPERTY(BlueprintReadOnly, Category = "Azure Kinect|Body")
	bool bIsTracked;

private:
	/** The Id of the body for a particular person index. */
	uint32 Id;

	/** Array of joints contained in this body. */
	UPROPERTY()
	TArray<FAzureKinectJoint> Joints;
};
