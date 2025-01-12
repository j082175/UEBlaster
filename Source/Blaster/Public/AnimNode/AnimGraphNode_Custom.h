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
//	// 초기화, 재초기화가 필요할 때 호출
//	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
//
//	// CacheBones 는 노드에 의해 레퍼런스 된 본 인덱스들을 리프레시 하는 데에 사용
//	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
//
//	// 현재 상황 업데이트시에 호출됨 (플레이 타임 증가나 블렌드 위젯 업데이트 등의 상황에서)
//	// 이 함수는 업데이트 시 현재 노드의 DeltaTime 과 blend weight 를 가지고 있는 FAnimationUpdateContext 를 입력받는다.
//	//virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
//
//	// Evaluate - 포즈를 만들어 내기 위해 호출됨 (Bone Transform's list)
//	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
//
//	//virtual void EvaluateComponentSpace_AnyThread(FComponentSpacePoseContext& Output) override;
//
//	// ShowDebug Animation 의 데이터를 디버깅하는데 사용
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
