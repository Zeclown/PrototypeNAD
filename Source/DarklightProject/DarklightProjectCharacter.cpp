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
	GrabbingRange = 100;
	MaxSlow = 0.1f;
	bIsInShadows = false;
	PrimaryActorTick.bCanEverTick = true;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

}

void ADarklightProjectCharacter::PostInitProperties()
{
	Super::PostInitProperties();
	Health = MaxHealth;
	InitialWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

}
#if WITH_EDITOR
void ADarklightProjectCharacter::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Health = MaxHealth;
	InitialWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
void ADarklightProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	for (TObjectIterator<ULightComponent> Itr; Itr; ++Itr)
	{
		// Filter out objects not contained in the target world.
		if (Itr->GetWorld() == GetWorld())
		{
			if (Itr->GetOwner()->ActorHasTag("KillingLight"))
			{
				Lights.Add(Itr->GetOwner());
				UE_LOG(LogTemp, Warning, TEXT("Adding as damaging light Actor:%s"), *Itr->GetOwner()->GetName());
			}
		}
	}
}
void ADarklightProjectCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	
}
void ADarklightProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	LightExposition = 0;
	FeetCoordinates = GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	ProcessLightStatus();
	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(InitialWalkSpeed*MaxSlow, InitialWalkSpeed, Health / MaxHealth);
}
float ADarklightProjectCharacter::EvaluateLightPotency(USpotLightComponent * LightComp)
{
		
	float DistanceFromLight = (FeetCoordinates - LightComp->GetOwner()->GetActorLocation()).Size();
	float LightPotency = 0;
	if (LightComp->AttenuationRadius*0.9f > DistanceFromLight)
	{
		FHitResult LightHitData;
		float Exposition;
		FVector ConeLine = LightComp->AttenuationRadius * LightComp->GetForwardVector();
		float RadiusAtEnd = FMath::Tan(FMath::DegreesToRadians(LightComp->OuterConeAngle)) * ConeLine.Size();
		bool bInsideCone = FMath::GetDistanceWithinConeSegment(FeetCoordinates, LightComp->GetComponentLocation(), ConeLine, 0, RadiusAtEnd, Exposition);
		if (bInsideCone && !GetWorld()->LineTraceSingleByChannel(LightHitData, FeetCoordinates, LightComp->GetComponentLocation(), ECC_Visibility))
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
	float DistanceFromLight = (FeetCoordinates - lightComp->GetOwner()->GetActorLocation()).Size();
	if (lightComp->AttenuationRadius*0.9f > DistanceFromLight)
	{
		FHitResult LightHitData;
		if (!GetWorld()->LineTraceSingleByChannel(LightHitData, FeetCoordinates, lightComp->GetOwner()->GetActorLocation(), ECC_Visibility))
		{
			//We calculate the light potency, which is stronger if the light is concentrated (small radius, big intensity) and vice versa
			LightPotency = lightComp->Intensity / lightComp->AttenuationRadius;
		}
	}		
	return LightPotency;
}

void ADarklightProjectCharacter::ProcessLightStatus()
{
	for (auto& light : Lights)
	{
		float DistanceFromLight = (GetActorLocation() - light->GetActorLocation()).Size();
		if (light->GetComponentByClass(USpotLightComponent::StaticClass()))// it is a spot light
		{
			USpotLightComponent* LightComp = Cast<USpotLightComponent>(light->GetComponentByClass(USpotLightComponent::StaticClass()));
			float Potency = EvaluateLightPotency(LightComp);
			LightExposition += Potency / DistanceFromLight * 100;
		}
		else if (light->GetComponentByClass(UPointLightComponent::StaticClass())) // it is a point light
		{
			UPointLightComponent* LightComp = Cast<UPointLightComponent>(light->GetComponentByClass(UPointLightComponent::StaticClass()));
			float Potency = EvaluateLightPotency(LightComp);
			LightExposition += Potency / DistanceFromLight * 100;
		}
	}
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


