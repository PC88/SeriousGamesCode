// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorWithLerp.h"
#include "Kismet/KismetMathLibrary.h"
#include "SpecificTriggerBox.h"


ADoorWithLerp::ADoorWithLerp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bPuzzleComplete = false;
	bOpen = false;

	MyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	MyBoxComponent->InitBoxExtent(FVector(500, 900, 600));
	RootComponent = MyBoxComponent;

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	Door->SetRelativeLocation(FVector(0.0f, 50.0f, -50.0f));
	Door->SetupAttachment(RootComponent);

	MyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoorWithLerp::OnOverlapBegin);
	MyBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADoorWithLerp::OnOverlapEnd);

}

// Called when the game starts or when spawned
void ADoorWithLerp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADoorWithLerp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoorRotation = Door->RelativeRotation;

	if (bOpen && SpecificActor && SpecificActor->bItemInPlace == true)
	{
		bPuzzleComplete = true;
		Door->SetRelativeRotation(FMath::Lerp(FQuat(DoorRotation), FQuat(FRotator(0.0f, RotateValue, 0.0f)), 0.01f));
	}
	else
	{
		Door->SetRelativeRotation(FMath::Lerp(FQuat(DoorRotation), FQuat(FRotator(0.0f, 0.0f, 0.0f)), 0.01f));
	}
}

void ADoorWithLerp::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		FVector PawnLocation = OtherActor->GetActorLocation();
		FVector Direction = GetActorLocation() - PawnLocation;
		Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, GetActorRotation());

		if (Direction.X > 0.0f)
		{
			RotateValue = 90.0f;
		}
		else
		{
			RotateValue = -90.0f;
		}

		bOpen = true;
	}
}

void ADoorWithLerp::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		bOpen = false;
	}
}

