// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueTestGameInstance.h"
UDialogueTestGameInstance::UDialogueTestGameInstance() {
	_DialogueSystem = NewObject<UDialogueSystem>(this, UDialogueSystem::StaticClass(), FName("DialogueSystem"));
}
UDialogueSystem* UDialogueTestGameInstance::GetDialogueSystem() {
	return _DialogueSystem;
}