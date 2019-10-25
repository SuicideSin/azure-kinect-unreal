// Fill out your copyright notice in the Description page of Project Settings.


#include "AzureKinectBody.h"

UAzureKinectBody::UAzureKinectBody() :
	Id(0)
{
}

UAzureKinectBody::~UAzureKinectBody()
{
}

int32 UAzureKinectBody::GetId() const
{
	return Id;
}

TArray<FAzureKinectJoint> UAzureKinectBody::GetJoints() const
{
	return Joints;
}

FAzureKinectJoint UAzureKinectBody::GetJoint(int32 index) const
{
	if (index < 0 || index >= Joints.Num()) return Joints[0];
	return Joints[index];
}
