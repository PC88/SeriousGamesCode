// Fill out your copyright notice in the Description page of Project Settings.

#include "SpecificTriggerBox.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))
#include "DrawDebugHelpers.h"



ASpecificTriggerBox::ASpecificTriggerBox()
{
	//Register Events
	OnActorBeginOverlap.AddDynamic(this, &ASpecificTriggerBox::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ASpecificTriggerBox::OnOverlapEnd);
	bItemInPlace = false;
}

// Called when the game starts or when spawned
void ASpecificTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	//DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Green, true, -1, 0, 5);
	//MyDelegate.BindStatic(&MyFunction, true, 20);
}

void ASpecificTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	//if the overlapping actor is the specific actor we identified in the editor
	if (OtherActor && (OtherActor != this) && OtherActor == SpecificActor)
	{
		print("Overlap Begin");
		printFString("Overlapping Actor = %s", *OtherActor->GetName());
		bItemInPlace = true;
		DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Green, true, -1, 0, 5);
	}
}

void ASpecificTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	//if the overlapping actor is the specific actor we identified in the editor
	if (OtherActor && (OtherActor != this) && OtherActor == SpecificActor)
	{
		print("Overlap End");
		printFString("%s has left the Trigger Box", *OtherActor->GetName());
		bItemInPlace = false;
		DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Red, true, -1, 0, 5);
	}
}

void ASpecificTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
