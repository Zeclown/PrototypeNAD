// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "DarklightProject.h"
#include "DarklightProjectCharacter.h"

ADarklightProjectCharacter::ADarklightProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	MaxHealth = 100;
	CurrentSlow = 0;
	PrimaryActorTick.bCanEverTick = true;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

}


void ADarklightProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsInShadows = false;
	Health = MaxHealth;
	for (TObjectIterator<ULightComponent> Itr; Itr; ++Itr)
	{
		// Filter out objects not contained in the target world.
		if (Itr->GetWorld() != GetWorld())
		{
			if (Itr->GetOwner() && Itr->GetOwner()->ActorHasTag("KillingLight"))
			{
				Lights.Add(Itr->GetOwner());
				UE_LOG(LogTemp, Warning, TEXT("Adding as damaging light Actor:%s"), *Itr->GetOwner()->GetName());
			}
		}
		// Do stuff
	}
}
void ADarklightProjectCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	
}
void ADarklightProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
float ADarklightProjectCharacter::EvaluateLightPotency(USpotLightComponent * LightComp)
{
		
	float DistanceFromLight = (GetActorLocation() - LightComp->GetOwner()->GetActorLocation()).Size();
	float LightPotency = 0;
	if (LightComp->AttenuationRadius*0.9f > DistanceFromLight)
	{
		FHitResult LightHitData;
		float Exposition;
		bool bInsideCone = FMath::GetDistanceWithinConeSegment(GetActorLocation(), LightComp->GetOwner()->GetActorLocation(), LightComp->AttenuationRadius*LightComp->GetOwner()->GetActorForwardVector(), 0, LightComp->OuterConeAngle, Exposition);
		if (bInsideCone && !GetWorld()->LineTraceSingleByChannel(LightHitData, GetActorLocation(), LightComp->GetOwner()->GetActorLocation(), ECC_Visibility))
		{
			//We calculate the light potency, which is stronger if the light is concentrated (small radius, big intensity) and vice versa
			LightPotency = LightComp->Intensity / LightComp->AttenuationRadius;
		}
	}
	return LightPotency;
}

float ADarklightProjectCharacter::EvaluateLightPotency(UPointLightComponent * lightComp)
{
	float LightPotency = 0;
	float DistanceFromLight = (GetActorLocation() - lightComp->GetOwner()->GetActorLocation()).Size();
	if (lightComp->AttenuationRadius*0.9f > DistanceFromLight)
	{
		FHitResult LightHitData;
		if (!GetWorld()->LineTraceSingleByChannel(LightHitData, GetActorLocation(), lightComp->GetOwner()->GetActorLocation(), ECC_Visibility))
		{
			//We calculate the light potency, which is stronger if the light is concentrated (small radius, big intensity) and vice versa
			LightPotency = lightComp->Intensity / lightComp->AttenuationRadius;
		}
	}		
	return LightPotency;
}



void ADarklightProjectCharacter::MoveForward(float Value)
{
	AddMovementInput(FVector(0.f, 1.f, 0.f), Value);
}

void ADarklightProjectCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(-1.f, 0.f, 0.f), Value);
}


