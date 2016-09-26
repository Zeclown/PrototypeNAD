// Fill out your copyright notice in the Description page of Project Settings.

#include "DarklightProject.h"
#include "LightWitch.h"

void ALightWitch::BeginPlay()
{
	Super::BeginPlay();
}

ALightWitch::ALightWitch()
{
	PrimaryActorTick.bCanEverTick = true;
}
void ALightWitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float Damage = 0;
	for (auto& light : Lights)
	{
		if (light->GetComponentByClass(USpotLightComponent::StaticClass()))// it is a spot light
		{
			USpotLightComponent* LightComp = Cast<USpotLightComponent>(light->GetComponentByClass(USpotLightComponent::StaticClass()));
			float Potency = EvaluateLightPotency(LightComp);
			float DistanceFromLight = (GetActorLocation() - LightComp->GetOwner()->GetActorLocation()).Size();
			Damage += KillingRatio*Potency / DistanceFromLight * 100;
		}
		else if (light->GetComponentByClass(UPointLightComponent::StaticClass())) // it is a point light
		{
			UPointLightComponent* LightComp = Cast<UPointLightComponent>(light->GetComponentByClass(UPointLightComponent::StaticClass()));
			float Potency = EvaluateLightPotency(LightComp);
			float DistanceFromLight = (GetActorLocation() - LightComp->GetOwner()->GetActorLocation()).Size();
			Damage += KillingRatio*Potency / DistanceFromLight * 100;
		}
	}
	Damage = LIGHT_TRESHOLD*DeltaTime - Damage;
	if (Damage > 0)
	{
		Health -= Damage*DeltaTime;
		bIsInShadows = true;
	}
	else
	{
		Health = MaxHealth;
		bIsInShadows = false;
	}
}
