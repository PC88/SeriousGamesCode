// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Pickup.generated.h"

UCLASS()
class ISHI419_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ActorMesh; // mesh visible in the editor -PC

	UPROPERTY(EditAnywhere)
	USceneComponent* HoldingComp; // The component being held -PC

	UFUNCTION()
		void Pickup(); 

	UFUNCTION()
		void RotateActor();

	bool bHolding;
	bool bGravity;

	FRotator ControlRotation; //Rotation gotten from playercontroller -PC

	ACharacter* MyCharacter;

	UCameraComponent* PlayerCamera;

	FVector ForwardVector;
	
};
