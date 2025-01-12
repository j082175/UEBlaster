// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNode/AnimGraphNode_Custom.h"
//#include "AnimGraphNode_BlendListBase.h"
//
//#define LOCTEXT_NAMESPACE "A3Nodes"
//
//void FRunAnimNode::Initialize_AnyThread(const FAnimationInitializeContext& Context)
//{
//
//	//UE_LOG(LogTemp, Display, TEXT("Initialize_AnyThread"));
//
//	ScaleMode = EBoneModificationMode::BMM_Replace;
//
//	if (bShowBone)
//	{
//		Scale = FVector(1.f);
//	}
//	else
//	{
//		Scale = FVector(0.f);
//	}
//
//	Super::Initialize_AnyThread(Context);
//}
//
//void FRunAnimNode::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
//{
//	Super::CacheBones_AnyThread(Context);
//	//UE_LOG(LogTemp, Display, TEXT("CacheBones_AnyThread"));
//	//BasePoseNode.CacheBones(Context);
//}
//
////void FRunAnimNode::Update_AnyThread(const FAnimationUpdateContext& Context)
////{
////	GetEvaluateGraphExposedInputs().Execute(Context);
////	BasePoseNode.Update(Context);
////}
//
//void FRunAnimNode::Evaluate_AnyThread(FPoseContext& Output)
//{
//
//	Super::Evaluate_AnyThread(Output);
//	//UE_LOG(LogTemp, Display, TEXT("Evaluate_AnyThread"));
//	//BasePoseNode.Evaluate(Output);
//}
//
//void FRunAnimNode::GatherDebugData(FNodeDebugData& DebugData)
//{
//	Super::GatherDebugData(DebugData);
//	//UE_LOG(LogTemp, Display, TEXT("GatherDebugData"));
//	FString DebugLine = DebugData.GetNodeName(this);
//
//	DebugData.AddDebugItem(DebugLine);
//	//BasePoseNode.GatherDebugData(DebugData);
//}
//
///// <summary>
///// /////////////////////////////////////////////////////////////
///// </summary>
///// <returns></returns>
///// 
//
//void UAnimGraphNode_Custom::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//
//	//UE_LOG(LogTemp, Display, TEXT("PostEditChangeProperty"));
//
//	const FName PropertyName = (PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None);
//
//	//if (MyNode.bShowPistolBone)
//	//{
//	//	MyNode.BoneToModify = MyNode.BoneToModify_Pistol;
//	//}
//	//else if (MyNode.bShowRifleBone)
//	//{
//	//	MyNode.BoneToModify = MyNode.BoneToModify_Rifle;
//	//}
//
//}
//
//FLinearColor UAnimGraphNode_Custom::GetNodeTitleColor() const
//{
//	return FLinearColor::Red;
//}
//
//FText UAnimGraphNode_Custom::GetTooltipText() const
//{
//	return LOCTEXT("ShowToolTip", "Tooltip");
//}
//
//FText UAnimGraphNode_Custom::GetNodeTitle(ENodeTitleType::Type TitleType) const
//{
//	return LOCTEXT("ABCDE", "ABC");
//}
//
//FString UAnimGraphNode_Custom::GetNodeCategory() const
//{
//	return TEXT("Create CustomNode");
//}
