// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "Components/BoxComponent.h"
#include "PortalTriggerSphere.generated.h"

/**
 * 
 */
UCLASS()
class ISHI419_API APortalTriggerSphere : public ATriggerSphere
{
	GENERATED_BODY()

protected:
       // Called when the game starts or when spawned
		virtual void BeginPlay() override;

public:

	// constructor sets default values for this actor's properties
	APortalTriggerSphere();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// overlap end function
	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	UBoxComponent* ExitBoxComponent;
	
};
