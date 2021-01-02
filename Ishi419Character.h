// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pickup.h"
#include "SwitchButton.h"
#include "Blueprint/UserWidget.h"
#include "Ishi419Character.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AIshi419Character : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

public:
	AIshi419Character();

	virtual void Tick(float DeltaTime) override; // this functions implementation has to call the Superclass`s tick function. it always has to in .cpp -PC

	virtual void BeginPlay();

	void PrintText();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	UPROPERTY(EditAnywhere)
	class UUserWidget* InfoWidget;


	UPROPERTY(EditAnywhere) // TODO PC //
	class USceneComponent* HoldingComponent;

	UPROPERTY(EditAnywhere) // TODO PC //
	class APickup* CurrentItem;

	UPROPERTY(Editanywhere)
	class ALerpControlButton* CurrentLerpButton;

	UPROPERTY(Editanywhere)
	class ASwitchButton* CurrentSwitchButton;

	UPROPERTY(EditAnywhere) // TODO PC //
	class ARotateIcon* CurrentIcon;

	bool bCanMove; // TODO PC //
	bool bHoldingItem; // TODO PC //
	bool bManipulating; // TODO PC //


	float PitchMax; // TODO PC //
	float PitchMin; // TODO PC //

	FVector HoldingComp;
	FRotator LastRotation; // this is so we can return the player back to normal when we are done rotating an object -PC

	FVector Start; // these are just for drawing DEBUG stuff -PC
	FVector ForwardVector; // TODO PC //
	FVector End; // TODO PC //

	FHitResult Hit; // what we get back when are done with the line trace -PC

	FComponentQueryParams DefaultComponentQueryParams; // TODO PC //
	FCollisionResponseParams DefaultResponseParams; // TODO PC //

	FTimerHandle InteractHandle; // displayed interact msg on timer -PC

protected:

	//-PC
	void OnInteract();

	//-PC
	void OnManipulate();

	//-PC
	void OnManipulateReleased();

	//-PC
	void ToggleIconRotation();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

	// TODO PC //
	void ToggleMovement();

	// TODO PC //
	void ToggleItemPickup();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/*
	* Configures input for touchscreen devices if there is a valid touch interface for doing so
	*
	* @param	InputComponent	The input component pointer to bind controls to
	* @returns true if touch controls were enabled.
	*/

public:
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

