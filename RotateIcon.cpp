// Fill out your copyright notice in the Description page of Project Settings.

#include "RotateIcon.h"
#include "DrawDebugHelpers.h"
#include "Ishi419Character.h"

// Sets default values
ARotateIcon::ARotateIcon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Actor Mesh"));
	IconMesh->SetSimulatePhysics(true);
	RootComponent = IconMesh;

	RotateValue = 90.0f;
	CurrentRotation = 0.0f;// defaults -PC
	DefaultRotation = 0.0f;
}

// Called when the game starts or when spawned
void ARotateIcon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARotateIcon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentRotation != DefaultRotation)
	{
		bCorrectRotation = true;
		//DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Green, true, -1, 0, 5); // THIS AND FPS ==NO WAY -PC
	}
	else
	{
		bCorrectRotation = false;
	}

	if (CurrentRotation >= 360) // manually wrapping the angle -PC
	{
		CurrentRotation = 0;
	}
}

void ARotateIcon::RotateIcon()
{
	IconMesh->SetRelativeRotation(FMath::Lerp(FQuat(IconRotation), FQuat(FRotator(0.0f, CurrentRotation += RotateValue, 0.0f)), 0.01f));
}
