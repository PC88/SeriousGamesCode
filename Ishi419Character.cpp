// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Ishi419Character.h"
#include "Ishi419Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "RotateIcon.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue,text)
#include "LerpControlButton.h"
#include "DrawDebugHelpers.h" // Included for Dubug tasks such as line trace -PC

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AIshi419Character


AIshi419Character::AIshi419Character()
{
	// REMOVED: GUN handling logic, Muzzle logic. -PC
	// REMOVED: VR Motion controller logic.
	// REMOVED: VR Gun logic
	// REMOVED: Gun off set
	// REMOVED: touchscreen logic.
	// REMOVED: OnFire method 

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//Sets default Values for bools and other relevant elements -PC

	HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	HoldingComponent->RelativeLocation.X = 50.0f;
	HoldingComponent->SetupAttachment(GetCapsuleComponent()); // needs some tweaking -PC

	CurrentIcon = NULL;
	CurrentItem = NULL;
	CurrentSwitchButton = NULL;
	CurrentLerpButton = NULL;

	bCanMove = true;
	bManipulating = false; // may again need tweaking -PC

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.
}

void AIshi419Character::Tick(float DeltaTime) // sets up tick on a designated basis, does not have to be frame by frame. -PC
{
	Super::Tick(DeltaTime);

	Start = FirstPersonCameraComponent->GetComponentLocation(); // DEBUG info, visual representation of the line trace will be going the way of the cameras forward vector -PC
	ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	End = ((ForwardVector * 200.0f) + Start);

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);


	if (!bHoldingItem) // if not holding check for somthing that could be held, else set the currentItem to NULL -PC
	{

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams,
			DefaultResponseParams))
		{
			if (Hit.GetActor()->GetClass()->IsChildOf(APickup::StaticClass()))
			{
				CurrentItem = Cast<APickup>(Hit.GetActor());
				GetWorld()->GetTimerManager().SetTimer(InteractHandle, this, &AIshi419Character::PrintText, 0.5f, false);
				InfoWidget->GetWidgetFromName("InteractImage")->SetVisibility(ESlateVisibility::Visible); // display widget -PC
			}
			if (Hit.GetActor()->GetClass()->IsChildOf(ARotateIcon::StaticClass()))
			{
				CurrentIcon = Cast<ARotateIcon>(Hit.GetActor());
				GetWorld()->GetTimerManager().SetTimer(InteractHandle, this, &AIshi419Character::PrintText, 0.5f, false);
				InfoWidget->GetWidgetFromName("InteractImage")->SetVisibility(ESlateVisibility::Visible); // display widget -PC
			}
			if (Hit.GetActor()->GetClass()->IsChildOf(ALerpControlButton::StaticClass()))
			{
				CurrentLerpButton = Cast<ALerpControlButton>(Hit.GetActor());
				GetWorld()->GetTimerManager().SetTimer(InteractHandle, this, &AIshi419Character::PrintText, 0.5f, false);
				InfoWidget->GetWidgetFromName("InteractImage")->SetVisibility(ESlateVisibility::Visible); // display widget -PC
			}
			if (Hit.GetActor()->GetClass()->IsChildOf(ASwitchButton::StaticClass()))
			{
				CurrentSwitchButton = Cast<ASwitchButton>(Hit.GetActor());
				GetWorld()->GetTimerManager().SetTimer(InteractHandle, this, &AIshi419Character::PrintText, 0.5f, false);
				InfoWidget->GetWidgetFromName("InteractImage")->SetVisibility(ESlateVisibility::Visible); // display widget -PC
			}
		}
		else
		{
			CurrentIcon = NULL;
			CurrentItem = NULL;
			CurrentLerpButton = NULL;
			CurrentSwitchButton = NULL;
			InfoWidget->GetWidgetFromName("InteractImage")->SetVisibility(ESlateVisibility::Hidden); // else hide widget -PC
		}
	}
	else
	{
		InfoWidget->GetWidgetFromName("InteractImage")->SetVisibility(ESlateVisibility::Hidden); // hide widget if holding -PC
	}

	if (bManipulating) // if inspecting is true and holding item is also true do logic, else retain FOV -PC
	{
		if (bHoldingItem)
		{
			FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(FirstPersonCameraComponent->FieldOfView, 90.0f, 0.1f));

			HoldingComponent->SetRelativeLocation(FVector(70.0f, 50.0f, 50.0f)); // if we are instpecting and holding is true, hold it infornt of the player -PC 
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.90000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -179.90000002f;
			CurrentItem->RotateActor(); // sets the current actors location to the controllers rotation i.e. the mouse. -PC
		}
		else
		{
			FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(FirstPersonCameraComponent->FieldOfView, 45.0f, 0.1f));
		}
	}
	else
	{
		FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(FirstPersonCameraComponent->FieldOfView,	90.0f, 0.1f));

		if (bHoldingItem)
		{
			HoldingComponent->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
		} 
	}
}

void AIshi419Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	PitchMax = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax; // setting pitch values -PC
	PitchMin = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin;

	if (InfoWidget)
	{
		InfoWidget->AddToViewport();
	}
}

// prints text on hover from macro - DEBUG -PC
void AIshi419Character::PrintText()
{
	print("Press 'E' to Interact");
	GetWorldTimerManager().ClearTimer(InteractHandle);
}


//////////////////////////////////////////////////////////////////////////
// Input

void AIshi419Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// This line *should* give us access to the current HUD object // EXAMPLE
	//AIshiHUD * hud = Cast<AIshiHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);


	PlayerInputComponent->BindAxis("MoveForward", this, &AIshi419Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AIshi419Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AIshi419Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AIshi419Character::LookUpAtRate);

	//Added bindings -PC
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AIshi419Character::OnInteract);

	PlayerInputComponent->BindAction("Manipulate", IE_Pressed, this, &AIshi419Character::OnManipulate);
	PlayerInputComponent->BindAction("Manipulate", IE_Released, this, &AIshi419Character::OnManipulateReleased);
}

void AIshi419Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AIshi419Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AIshi419Character::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void AIshi419Character::MoveForward(float Value)
{
	if (Value != 0.0f && bCanMove) // modified to halt moving -PC
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AIshi419Character::MoveRight(float Value)
{
	if (Value != 0.0f && bCanMove) // modified to halt moving -PC
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AIshi419Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AIshi419Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//Calls relevant Delegates, or otherforms of class communication -PC
void AIshi419Character::OnInteract() 
{
	if (CurrentItem && !bManipulating)
	{
		ToggleItemPickup(); 
	}
	if (CurrentIcon)
	{
		ToggleIconRotation();
	}
	if (CurrentLerpButton)
	{
		CurrentLerpButton->bIsBeingPressed = true;
	}
	if (CurrentSwitchButton)
	{
		if (CurrentSwitchButton->bPressed == false)
		{
			CurrentSwitchButton->ToggleButton();
		}
		else
		{
			CurrentSwitchButton->ResetToggleButton();
		}
	}
}

void AIshi419Character::OnManipulate()
{
	if (bHoldingItem)
	{
		LastRotation = GetControlRotation(); // TODO 42:43
		ToggleMovement();
	}
	else
	{
		bManipulating = true;
	}
}

void AIshi419Character::OnManipulateReleased()
{
	if (bManipulating && bHoldingItem)
	{
		GetController()->SetControlRotation(LastRotation);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = PitchMax;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = PitchMin;
		ToggleMovement();
	}
	else
	{
		bManipulating = false;
	}
}

void AIshi419Character::ToggleMovement()
{
	bCanMove = !bCanMove;
	bManipulating = !bManipulating;
	FirstPersonCameraComponent->bUsePawnControlRotation = !FirstPersonCameraComponent->bUsePawnControlRotation;
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
}

void AIshi419Character::ToggleItemPickup() //Delagate to pickup Actor
{
	if (CurrentItem)
	{
		bHoldingItem = !bHoldingItem;
		CurrentItem->Pickup();

		if (!bHoldingItem)
		{
			CurrentItem = NULL;
		}
	}
}

void AIshi419Character::ToggleIconRotation()
{
	if (CurrentIcon)
	{
		CurrentIcon->RotateIcon();
	}
}