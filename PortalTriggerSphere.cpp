// Fill out your copyright notice in the Description page of Project Settings.

#include "PortalTriggerSphere.h"
#include "DrawDebugHelpers.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))




APortalTriggerSphere::APortalTriggerSphere()
{
	//Register Events
	OnActorBeginOverlap.AddDynamic(this, &APortalTriggerSphere::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &APortalTriggerSphere::OnOverlapEnd);

	ExitBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Exit Box Component"));
	ExitBoxComponent->InitBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	ExitBoxComponent->SetRelativeLocation(FVector(0.0f, 100.0f, -50.0f));
	ExitBoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APortalTriggerSphere::BeginPlay()
{
	Super::BeginPlay();

}

void APortalTriggerSphere::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	//if the overlapping actor is the specific actor we identified in the editor
	if (OtherActor && (OtherActor != this))
	{
		FVector DesiredLocation;
		DesiredLocation = ExitBoxComponent->GetComponentLocation();
		print("Overlap Begin");
		printFString("Overlapping Actor = %s", *OtherActor->GetName());
		DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Green, true, -1, 0, 5);
		OtherActor->SetActorLocation(DesiredLocation);
	}
}

void APortalTriggerSphere::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	//if the overlapping actor is the specific actor we identified in the editor
	if (OtherActor && (OtherActor != this))
	{
		print("Overlap End");
		printFString("%s has left the Trigger Box", *OtherActor->GetName());
		DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Red, true, -1, 0, 5);
	}
}

void APortalTriggerSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



