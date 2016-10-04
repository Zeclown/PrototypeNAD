// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "DarklightProjectCharacter.generated.h"

UCLASS(config=Game)
class ADarklightProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override ;
	float EvaluateLightPotency(USpotLightComponent* lightComp);
	float EvaluateLightPotency(UPointLightComponent* lightComp);
	void ProcessLightStatus();
	// End of APawn interface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerShadowState)
	TArray<AActor*> Lights;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerState")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
	float KillingRatio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
	bool bIsGrabbing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState")
	float CurrentSlow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState")
	float LightExposition;
	float InitialWalkSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState")
	FVector FeetCoordinates;
	/**Tick event called by the blueprint */
	void Tick(float deltaTime);


public:
	ADarklightProjectCharacter();
	virtual void PostInitProperties();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
	virtual void BeginPlay();
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Called for front to back input*/
	void MoveForward(float Val);
	/** Called for side to side input */
	void MoveRight(float Val);
	/**Is the character standing in the shadow? Modified in the tick function */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState")
	bool bIsInShadows;
	/**Max slow that can be applied to the character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState", meta = (UIMin = 0.0, UIMax = 1.0, ClampMax=1.0,ClampMin=0.0))
	float MaxSlow;



};
