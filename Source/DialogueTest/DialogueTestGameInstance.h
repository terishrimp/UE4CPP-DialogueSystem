// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DialogueTest/DialogueSystem/DialogueSystem.h"
#include "DialogueTestGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUETEST_API UDialogueTestGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UDialogueSystem* _DialogueSystem;

public:
	UDialogueTestGameInstance();
	UDialogueSystem* GetDialogueSystem();
};
