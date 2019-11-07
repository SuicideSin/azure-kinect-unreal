// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "k4abt.h"
#include "AzureKinectBody.generated.h"


USTRUCT(BlueprintType, Category = "Azure Kinect|Body|Joints")
struct FAzureKinectJoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Azure Kinect|Body|Joints")
	FVector Position;

	UPROPERTY(BlueprintReadOnly, Category = "Azure Kinect|Body|Joints")
	FRotator Orientation;

	float PositionMultiplier = 0.5f;

	UPROPERTY(BlueprintReadOnly, Category = "Azure Kinect|Body|Joints")
	int32 Index;

	FAzureKinectJoint() :
		Position(FVector::ZeroVector),
		Orientation(FRotator::ZeroRotator),
		Index(0)
	{
	}

	void UpdateFromNativeJoint(const k4abt_joint_t &NativeJoint, bool IsMirrored = false)
	{
		int32 XMultiplier = 1;	// (IsMirrored ? 1 : -1);
		Position.Set(NativeJoint.position.xyz.z * PositionMultiplier * XMultiplier, NativeJoint.position.xyz.x * PositionMultiplier, -NativeJoint.position.xyz.y * PositionMultiplier);

		FQuat JointQuaternion = FQuat(
			-NativeJoint.orientation.wxyz.z,
			-NativeJoint.orientation.wxyz.x,
			NativeJoint.orientation.wxyz.y,
			NativeJoint.orientation.wxyz.w);

#define JOINT_ID(Name) static_cast<uint8>(k4abt_joint_id_t::Name)

		if (Index <= JOINT_ID(K4ABT_JOINT_NECK) 
			|| (Index >= JOINT_ID(K4ABT_JOINT_HEAD) && Index <= JOINT_ID(K4ABT_JOINT_EAR_RIGHT))
			|| (Index >= JOINT_ID(K4ABT_JOINT_HIP_LEFT) && Index <= JOINT_ID(K4ABT_JOINT_FOOT_LEFT)))
		{
			Orientation = FRotationMatrix::MakeFromXZ(JointQuaternion.GetAxisY(), -JointQuaternion.GetAxisX())
				.Rotator();

			//Orientation = JointQuaternion.Rotator();
		}
		//else if (Index >= JOINT_ID(K4ABT_JOINT_HIP_LEFT) && Index <= JOINT_ID(K4ABT_JOINT_FOOT_RIGHT))
		//{
		//	Orientation = FRotationMatrix::MakeFromXZ(JointQuaternion.GetAxisY(), JointQuaternion.GetAxisZ())
		//		.Rotator();
		//}
		else if (Index >= JOINT_ID(K4ABT_JOINT_HIP_RIGHT) && Index <= JOINT_ID(K4ABT_JOINT_FOOT_RIGHT))
		{
			Orientation = FRotationMatrix::MakeFromXZ(JointQuaternion.GetAxisY(), -JointQuaternion.GetAxisX())
				.Rotator();
		}
		//else if (Index == JOINT_ID(K4ABT_JOINT_CLAVICLE_LEFT))	// || Index == JOINT_ID(K4ABT_JOINT_SHOULDER_LEFT))
		//{
		//	Orientation = FRotationMatrix::MakeFromXZ(-JointQuaternion.GetAxisX(), JointQuaternion.GetAxisZ())
		//		.Rotator();

		//	//Orientation = JointQuaternion.Rotator();
		//}
		else if (Index == JOINT_ID(K4ABT_JOINT_SHOULDER_LEFT))	// || Index == JOINT_ID(K4ABT_JOINT_ELBOW_LEFT))
		{
			//Orientation = FRotationMatrix::MakeFromYZ(-JointQuaternion.GetAxisX(), JointQuaternion.GetAxisZ())
			//	.Rotator();

			Orientation = FRotationMatrix::MakeFromXZ(JointQuaternion.GetAxisY(), JointQuaternion.GetAxisZ())
				.Rotator();
		}
		else if (Index == JOINT_ID(K4ABT_JOINT_ELBOW_LEFT))
		{
			Orientation = FRotationMatrix::MakeFromXZ(JointQuaternion.GetAxisY(), -JointQuaternion.GetAxisZ())
				.Rotator();

			//Orientation = FRotationMatrix::MakeFromYZ(JointQuaternion.GetAxisX(), -JointQuaternion.GetAxisZ())
			//	.Rotator();

			//Orientation = JointQuaternion.Rotator();

		}
		else if (Index == JOINT_ID(K4ABT_JOINT_WRIST_LEFT))
		{
			//Orientation = FRotationMatrix::MakeFromYZ(JointQuaternion.GetAxisX(), -JointQuaternion.GetAxisZ())
			//	.Rotator();

			Orientation = FRotationMatrix::MakeFromXZ(JointQuaternion.GetAxisY() , -JointQuaternion.GetAxisZ())
				.Rotator();

			//Orientation = JointQuaternion.Rotator();
		}
		//else if (Index == JOINT_ID(K4ABT_JOINT_CLAVICLE_RIGHT))	// || Index == JOINT_ID(K4ABT_JOINT_SHOULDER_RIGHT))
		//{
		//	Orientation = FRotationMatrix::MakeFromXZ(-JointQuaternion.GetAxisX(), JointQuaternion.GetAxisZ())
		//		.Rotator();
		//}
		else if (Index == JOINT_ID(K4ABT_JOINT_SHOULDER_RIGHT))	// || Index == JOINT_ID(K4ABT_JOINT_ELBOW_RIGHT))
		{
			//Orientation = FRotationMatrix::MakeFromYZ(JointQuaternion.GetAxisX(), JointQuaternion.GetAxisZ())
			//	.Rotator();

			Orientation = FRotationMatrix::MakeFromXZ(JointQuaternion.GetAxisY(), JointQuaternion.GetAxisZ())
				.Rotator();
		}
		else if (Index == JOINT_ID(K4ABT_JOINT_ELBOW_RIGHT))
		{
			Orientation = FRotationMatrix::MakeFromXZ(JointQuaternion.GetAxisY(), -JointQuaternion.GetAxisZ())
				.Rotator();
		}
		else if (Index == JOINT_ID(K4ABT_JOINT_WRIST_RIGHT))
		{
			//Orientation = FRotationMatrix::MakeFromYZ(JointQuaternion.GetAxisX(), -JointQuaternion.GetAxisZ())
			//	.Rotator();

			Orientation = FRotationMatrix::MakeFromXZ(JointQuaternion.GetAxisY(), -JointQuaternion.GetAxisZ())
				.Rotator();

			//Orientation = JointQuaternion.Rotator();
		}

#undef JOINT_ID
	}
};


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
 * 
 */
UCLASS(BlueprintType, Category = "Azure Kinect|Body")
class AZUREKINECTUNREAL_API UAzureKinectBody : public UObject
{
	GENERATED_BODY()
	
public:
	UAzureKinectBody();
	~UAzureKinectBody();

	UFUNCTION(BlueprintPure, Category = "Azure Kinect|Body")
	int32 GetId() const;

	UFUNCTION(BlueprintPure, Category = "Azure Kinect|Body|Joints")
	TArray<FAzureKinectJoint> GetJoints() const;

	UFUNCTION(BlueprintPure, Category = "Azure Kinect|Body|Joints")
	FAzureKinectJoint GetJoint(int32 index) const;

	void UpdateBodyWithKinectInfo(const k4abt_body_t &NativeBody, bool IsTracked);

	UPROPERTY(BlueprintReadWrite, Category = "Azure Kinect|Body")
	bool bIsMirrored;

	UPROPERTY(BlueprintReadOnly, Category = "Azure Kinect|Body")
	bool bIsTracked;

private:

	uint32 Id;

	UPROPERTY()
	TArray<FAzureKinectJoint> Joints;

	const uint32 JointCount = K4ABT_JOINT_COUNT;

	//k4abt_skeleton_t NativeSkeleton;
};
