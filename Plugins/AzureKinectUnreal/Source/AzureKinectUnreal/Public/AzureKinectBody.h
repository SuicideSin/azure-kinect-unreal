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

	UPROPERTY(BlueprintReadWrite, Category = "Azure Kinect|Body|Joints")
	float PositionMultiplier = 0.5f;

	FAzureKinectJoint() :
		Position(FVector::ZeroVector),
		Orientation(FRotator::ZeroRotator)
	{
	}

	void UpdateFromNativeJoint(const k4abt_joint_t &NativeJoint, bool IsMirrored = false)
	{
		int32 XMultiplier = (IsMirrored ? 1 : -1);
		Position.Set(NativeJoint.position.xyz.z * PositionMultiplier * XMultiplier, -NativeJoint.position.xyz.x * PositionMultiplier, -NativeJoint.position.xyz.y * PositionMultiplier);

		FQuat JointQuaternion = FQuat(
			-NativeJoint.orientation.wxyz.z,
			NativeJoint.orientation.wxyz.x,
			-NativeJoint.orientation.wxyz.y,
			NativeJoint.orientation.wxyz.w);
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

	void UpdateBodyWithKinectInfo(const k4abt_body_t &NativeBody);

	UPROPERTY(BlueprintReadWrite, Category = "Azure Kinect|Body")
	bool bIsMirrored;

private:

	uint32 Id;

	UPROPERTY()
	TArray<FAzureKinectJoint> Joints;

	const uint32 JointCount = K4ABT_JOINT_COUNT;

	//k4abt_skeleton_t NativeSkeleton;
};
