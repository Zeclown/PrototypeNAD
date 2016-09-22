

#pragma once

#include "DarklightProjectCharacter.h"
#include "DarkWitch.generated.h"

/**
 * 
 */
UCLASS()
class DARKLIGHTPROJECT_API ADarkWitch : public ADarklightProjectCharacter
{
	GENERATED_BODY()
protected:
	/**Tick event called by the blueprint */
	void Tick(float deltaTime);
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	void BeginPlay();
	//The light counterpart so we can set its inputs
	ADarklightProjectCharacter* LightCharacter;
public:
	ADarkWitch();

};
