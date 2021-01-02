// Fill out your copyright notice in the Description page of Project Settings.

#include "CVDTestTriggerSphere.h"
#include "DrawDebugHelpers.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))


ACVDTestTriggerSphere::ACVDTestTriggerSphere()
{
	//Register Events
	OnActorBeginOverlap.AddDynamic(this, &ACVDTestTriggerSphere::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACVDTestTriggerSphere::OnOverlapEnd);

	DisplayIcon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	DisplayIcon->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	DisplayIcon->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ACVDTestTriggerSphere::BeginPlay()
{
	Super::BeginPlay();

}

void ACVDTestTriggerSphere::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	//if the overlapping actor is the specific actor we identified in the editor
	if (OtherActor && (OtherActor != this))
	{
		print("Overlap Begin");
		printFString("Overlapping Actor = %s", *OtherActor->GetName());
		DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Green, true, -1, 0, 5);
	}

	for (int i = 0; i < IconList.Num(); i++)
	{
		if (IconList[i]->CurrentRotation != 90.0f)
		{
			DisplayIcon->SetRelativeRotation(FMath::Lerp(FQuat(IconRotation), FQuat(FRotator(0.0f, CVDRotation, 0.0f)), 0.01f));
		}
	}
}

void ACVDTestTriggerSphere::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	//if the overlapping actor is the specific actor we identified in the editor
	if (OtherActor && (OtherActor != this))
	{
		print("Overlap End");
		printFString("%s has left the Trigger Box", *OtherActor->GetName());
		DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Red, true, -1, 0, 5);
	}
}

void ACVDTestTriggerSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


