// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleDoorWithLerp.h"
#include "Kismet/KismetMathLibrary.h"
#include "PuzzleIcon.h"


APuzzleDoorWithLerp::APuzzleDoorWithLerp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bPuzzleComplete = false;
	bOpen = false;
	RootComponent = MyBoxComponent; // LESSON LEARNED NOT HOW INHERITANCE WORKS -PC

	MyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APuzzleDoorWithLerp::OnOverlapBegin);
	MyBoxComponent->OnComponentEndOverlap.AddDynamic(this, &APuzzleDoorWithLerp::OnOverlapEnd);

}

// Called when the game starts or when spawned
void APuzzleDoorWithLerp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APuzzleDoorWithLerp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DoorRotation = Door->RelativeRotation;

	if (bOpen && SpecificActorOne && SpecificActorOne->bCorrectRotation == true
		&& SpecificActorTwo && SpecificActorTwo->bCorrectRotation == true
		&& SpecificActorThree && SpecificActorThree->bCorrectRotation == true
		&& SpecificActorFour && SpecificActorFour->bCorrectRotation == true)
	{
		bPuzzleComplete = true;
		Door->SetRelativeRotation(FMath::Lerp(FQuat(DoorRotation), FQuat(FRotator(0.0f, RotateValue, 0.0f)), 0.01f));
	}
	else
	{
		Door->SetRelativeRotation(FMath::Lerp(FQuat(DoorRotation), FQuat(FRotator(0.0f, 0.0f, 0.0f)), 0.01f));
	}
}

void APuzzleDoorWithLerp::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		FVector PawnLocation = OtherActor->GetActorLocation();
		FVector Direction = GetActorLocation() - PawnLocation;
		Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, GetActorRotation());

		if (Direction.X > 0.0f)
		{
			RotateValue = 180.0f;
		}
		else
		{
			RotateValue = -180.0f;
		}

		bOpen = true;
	}
}

void APuzzleDoorWithLerp::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		bOpen = false;
	}
}



