// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "DialogueTest/DialogueSystem/DialogueSystem.h"
#include "Kismet/GameplayStatics.h"
#include "DialogueTest/Character/CharacterBase.h"
#include "DialogueTest/DialogueTestGameInstance.h"
#include "DialogueInteractable.generated.h"

/**
 *
 */

UCLASS()
class DIALOGUETEST_API ADialogueInteractable : public AInteractable
{
	GENERATED_BODY()

public:
	//Functions
	ADialogueInteractable();
	bool Execute(APawn* Executor) override;
	float GetTextSpeed();
protected:
	//Exposed variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		UDataTable* _Dialogue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float _TextSpeed;

	//Variables
	UPROPERTY()
		UDialogueSystem* _DialogueSystem;

	UPROPERTY()
		FDialogueInfo _DialogueInfo;

	UPROPERTY()
		ACharacterBase* _Player;

	//Functions
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
