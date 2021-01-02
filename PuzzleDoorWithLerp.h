// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoorWithLerp.h"
#include "RotateIcon.h"
#include "PuzzleDoorWithLerp.generated.h"

/**
 * 
 */
UCLASS()
class ISHI419_API APuzzleDoorWithLerp : public ADoorWithLerp
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APuzzleDoorWithLerp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// specific actor for overlap
	UPROPERTY(EditAnywhere)
		class ARotateIcon* SpecificActorOne;

	// specific actor for overlap
	UPROPERTY(EditAnywhere)
		class ARotateIcon* SpecificActorTwo;

	// specific actor for overlap
	UPROPERTY(EditAnywhere)
		class ARotateIcon* SpecificActorThree;

	// specific actor for overlap
	UPROPERTY(EditAnywhere)
		class ARotateIcon* SpecificActorFour;


	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	
};
