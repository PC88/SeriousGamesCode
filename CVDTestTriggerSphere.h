// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "RotateIcon.h"
#include "Containers/Array.h"
#include "CVDTestTriggerSphere.generated.h"
/**
 * 
 */
UCLASS()
class ISHI419_API ACVDTestTriggerSphere : public ATriggerSphere
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// constructor sets default values for this actor's properties
	ACVDTestTriggerSphere();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// overlap end function
	UFUNCTION()
		void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY(EditAnyWhere)
		float CVDRotation;

	UPROPERTY(EditAnyWhere)
		float DefaultRotation;

	FQuat IconRotation;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DisplayIcon;

	// list of icons to check rotations
	UPROPERTY(EditAnywhere)
	TArray<ARotateIcon*> IconList;
};
