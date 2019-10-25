// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AzureKinectBody.generated.h"


USTRUCT(BlueprintType, Category = "Azure Kinect|Body|Joints")
struct FAzureKinectJoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Azure Kinect|Body|Joints")
	FVector Position;

	UPROPERTY(BlueprintReadOnly, Category = "Azure Kinect|Body|Joints")
	FRotator Orientation;

	FAzureKinectJoint() :
		Position(FVector::ZeroVector),
		Orientation(FRotator::ZeroRotator)
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

	UFUNCTION(BlueprintCallable, Category = "Azure Kinect|Body")
	int32 GetId() const;

	UFUNCTION(BlueprintCallable, Category = "Azure Kinect|Body|Joints")
	TArray<FAzureKinectJoint> GetJoints() const;

	UFUNCTION(BlueprintCallable, Category = "Azure Kinect|Body|Joints")
	FAzureKinectJoint GetJoint(int32 index) const;

private:

	uint32 Id;

	UPROPERTY()
	TArray<FAzureKinectJoint> Joints;
};
