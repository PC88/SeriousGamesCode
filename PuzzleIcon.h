// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleIcon.generated.h"

UCLASS()
class ISHI419_API APuzzleIcon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleIcon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* IconMesh;

	UPROPERTY(EditAnyWhere)
		float RotateValue;

	UPROPERTY(EditAnyWhere)
		float CurrentRotation;

	UPROPERTY(EditAnyWhere)
		float DefaultRotation;

	bool bCorrectRotation;

	FQuat IconRotation; // If this is a rotator it WILL NOT WRAP, and be stuck at 90 degrees -PC

	void RotateIcon();
};
