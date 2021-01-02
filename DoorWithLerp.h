// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "DoorWithLerp.generated.h"

UCLASS()
class ISHI419_API ADoorWithLerp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoorWithLerp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Door;

	UPROPERTY(EditAnywhere)
		UBoxComponent* MyBoxComponent;

	// specific actor for overlap
	UPROPERTY(EditAnywhere)
		class ASpecificTriggerBox* SpecificActor;

	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool bOpen;

	bool bPuzzleComplete;

	float RotateValue;

	FRotator DoorRotation;
	
};
