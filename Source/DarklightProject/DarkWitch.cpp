// Fill out your copyright notice in the Description page of Project Settings.

#include "DarklightProject.h"
#include "DarkWitch.h"
#include "LightWitch.h"
ADarkWitch::ADarkWitch()
{
	PrimaryActorTick.bCanEverTick = true;

}
void ADarkWitch::SetupPlayerInputComponent(UInputComponent * InputComponent)
{
	for (TObjectIterator<ALightWitch> Itr; Itr; ++Itr)
	{
		// Filter out objects not contained in the target world.
		if (Itr->GetWorld() != GetWorld())
		{
			LightCharacter = *Itr;
		}
		// Do stuff
	}
	//////////////////////////////////////////////////////////////////////////
	// Input
	// Set up gameplay key bindings
	/////////////////////////////////////////////////////////////////////////
	InputComponent->BindAction("JumpShadow", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("JumpShadow", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAxis("MoveRightShadow", this, &ADarklightProjectCharacter::MoveRight);
	InputComponent->BindAxis("MoveForwardShadow", this, &ADarklightProjectCharacter::MoveForward);

}

void ADarkWitch::BeginPlay()
{
	Super::BeginPlay();

}

void ADarkWitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bool InDanger = false;
	float Damage = 0;
	for (auto& light : Lights)
	{
		if (light->GetComponentByClass(USpotLightComponent::StaticClass()))// it is a spot light
		{
			USpotLightComponent* LightComp = Cast<USpotLightComponent>(light->GetComponentByClass(USpotLightComponent::StaticClass()));
			float Potency=EvaluateLightPotency(LightComp);
			float DistanceFromLight = (GetActorLocation() - LightComp->GetOwner()->GetActorLocation()).Size();
			Damage = KillingRatio*DeltaTime*Potency / DistanceFromLight * 100;
			if(Damage>0)
				InDanger = true;							
		}
		else if (light->GetComponentByClass(UPointLightComponent::StaticClass())) // it is a point light
		{
			UPointLightComponent* LightComp = Cast<UPointLightComponent>(light->GetComponentByClass(UPointLightComponent::StaticClass()));
			float Potency = EvaluateLightPotency(LightComp);
			float DistanceFromLight = (GetActorLocation() - LightComp->GetOwner()->GetActorLocation()).Size();
			Damage = KillingRatio*DeltaTime*Potency / DistanceFromLight * 100;
			if (Damage>0)
				InDanger = true;
		}
	}

	Health -= Damage;
	if (!InDanger)
	{
		Health = MaxHealth;
		bIsInShadows = true;
	}

	

}