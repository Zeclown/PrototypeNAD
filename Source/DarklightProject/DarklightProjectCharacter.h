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
	// End of APawn interface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerShadowState)
		TArray<AActor*> Lights;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerState")
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
		float KillingRatio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float MaxMovementDepth;
	float CurrentSlow;
	/**Tick event called by the blueprint */
	void Tick(float deltaTime);


public:
	ADarklightProjectCharacter();
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerShadowState)
		bool bIsInShadows;
	/**Is the character standing in the shadow? Modified in the tick function */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerShadowState)
		float SlowRatio;
	/**Is the character standing in the shadow? Modified in the tick function */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerShadowState)
		float MaxSlow;



};
