// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setting Default Values -PC
	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Actor Mesh"));
	ActorMesh->SetSimulatePhysics(true);
	RootComponent = ActorMesh;

	bHolding = false;
	bGravity = true;

}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
	MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	PlayerCamera = MyCharacter->FindComponentByClass<UCameraComponent>();

	TArray<USceneComponent*> Components;

	MyCharacter->GetComponents(Components); //places MyCharacter in to the above TArray of USceneComponents -PC

	if (Components.Num() > 0) // Null check in effect -PC
	{
		for (auto& Comp : Components) // auto here avoids any type errors, defensive programming, other wise populates the Array -PC
		{
			if (Comp->GetName() == "HoldingComponent")
			{
				HoldingComp = Cast<USceneComponent>(Comp);
			}
		}
	}
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHolding && HoldingComp)
	{
		SetActorLocationAndRotation(HoldingComp->GetComponentLocation(), HoldingComp->GetComponentRotation());
	}
}

// Rotates the given actor when relevant -PC
void APickup::RotateActor()
{
	ControlRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	SetActorRotation(FQuat(ControlRotation));
}

// this is not a constructor the constructor is "APickup" -PC
void APickup::Pickup()
{
	bHolding = !bHolding;
	bGravity = !bGravity;
	ActorMesh->SetEnableGravity(bGravity);
	ActorMesh->SetSimulatePhysics(bHolding ? false : true); // Terenary Operator, if bHolding is true, SSphysics is false, if false vice versa -PC
	ActorMesh->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics); // Terenary Operator, if bHolding is true, No Collision, if false vice versa -PC

	if (!bHolding)
	{
		ForwardVector = PlayerCamera->GetForwardVector();
		ActorMesh->AddForce(ForwardVector * ActorMesh->GetMass()); // Adds inertia to the actor this may need changes/tweaked -PC
	}
}
