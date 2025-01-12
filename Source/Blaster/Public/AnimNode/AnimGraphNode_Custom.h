//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
////#include "AnimGraphNode_Base.h"
//#include "AnimGraphNode_ModifyBone.h"
//#include "AnimGraphNode_Custom.generated.h"
//
///**
// *
// */
//USTRUCT(BlueprintType)
//struct FRunAnimNode : public FAnimNode_ModifyBone
//{
//	GENERATED_BODY()
//public:
//
//	// �ʱ�ȭ, ���ʱ�ȭ�� �ʿ��� �� ȣ��
//	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
//
//	// CacheBones �� ��忡 ���� ���۷��� �� �� �ε������� �������� �ϴ� ���� ���
//	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
//
//	// ���� ��Ȳ ������Ʈ�ÿ� ȣ��� (�÷��� Ÿ�� ������ ���� ���� ������Ʈ ���� ��Ȳ����)
//	// �� �Լ��� ������Ʈ �� ���� ����� DeltaTime �� blend weight �� ������ �ִ� FAnimationUpdateContext �� �Է¹޴´�.
//	//virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
//
//	// Evaluate - ��� ����� ���� ���� ȣ��� (Bone Transform's list)
//	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
//
//	//virtual void EvaluateComponentSpace_AnyThread(FComponentSpacePoseContext& Output) override;
//
//	// ShowDebug Animation �� �����͸� ������ϴµ� ���
//	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
//
//	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Links")
//	//FPoseLink BasePoseNode;
//
//	//UPROPERTY(EditAnywhere, meta = (PinShownByDefault))
//	//mutable uint8 bShowRifleBone : 1;
//
//	//UPROPERTY(EditAnywhere, meta = (PinShownByDefault))
//	//mutable uint8 bShowPistolBone : 1;
//
//	//UPROPERTY(EditAnywhere, Category = SkeletalControl)
//	//FBoneReference BoneToModify_Rifle;
//
//	//UPROPERTY(EditAnywhere, Category = SkeletalControl)
//	//FBoneReference BoneToModify_Pistol;
//
//	UPROPERTY(EditAnywhere, meta = (PinShownByDefault))
//	mutable uint8 bShowBone : 1;
//};
//
//
//UCLASS(BlueprintType)
//class BLASTER_API UAnimGraphNode_Custom : public UAnimGraphNode_ModifyBone
//{
//	GENERATED_BODY()
//public:
//	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
//
//	// GraphNode's color
//	virtual FLinearColor GetNodeTitleColor() const override;
//
//	// Show GraphNode's ToolTip
//	virtual FText GetTooltipText() const override;
//
//	// GraphNode's Name
//	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
//
//	// Set up GraphNode's Category
//	virtual FString GetNodeCategory() const override;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	FRunAnimNode MyNode;
//};
