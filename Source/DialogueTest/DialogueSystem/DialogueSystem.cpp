// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueSystem.h"

const FDialogueInfo UDialogueSystem::NullDialogueInfo = FDialogueInfo();

UReadingState* UDialogueSystem::_ReadingState;
UCompleteState* UDialogueSystem::_CompleteState;
UPauseState* UDialogueSystem::_PauseState;

UDialogueSystem::UDialogueSystem() {

	//AddToRoot is done so objects referenced by static pointers are never destroyed
	if (!UDialogueSystem::_ReadingState) {
		UDialogueSystem::_ReadingState = CreateDefaultSubobject<UReadingState>(TEXT("ReadingState"));
		UDialogueSystem::_ReadingState->AddToRoot();
		UE_LOG(LogTemp, Log, TEXT("%s"), *UDialogueSystem::_ReadingState->GetName());
	}
	if (!UDialogueSystem::_CompleteState) {
		UDialogueSystem::_CompleteState = CreateDefaultSubobject<UCompleteState>(TEXT("CompleteState"));
		UDialogueSystem::_CompleteState->AddToRoot();
		UE_LOG(LogTemp, Log, TEXT("%s"), *UDialogueSystem::_CompleteState->GetName());
	}
	if (!UDialogueSystem::_PauseState) {
		UDialogueSystem::_PauseState = CreateDefaultSubobject<UPauseState>(TEXT("PauseState"));
		UDialogueSystem::_PauseState->AddToRoot();
		UE_LOG(LogTemp, Log, TEXT("%s"), *UDialogueSystem::_PauseState->GetName());
	}

	_DialogueState = UDialogueSystem::_CompleteState;
	_Status = EDialogueStat::Complete;
	_DialogueInfoIndex = 1;
	_CurrentCharIndex = 0;
}

void UDialogueSystem::SetStatus_Implementation(UDialogueState* const DialogueState)
{
	if (_DialogueState) {
		_DialogueState = DialogueState;
		DialogueState->Enter(*this);
	}
}

void UDialogueSystem::OnPlayerExecute() {
	if (_DialogueState) {
		_DialogueState = _DialogueState->HandleInput(*this);
		_DialogueState->Enter(*this);
	}
}

FDialogueInfo UDialogueSystem::GetDialogueInfo() {
	if (_Dialogue) {
		FString ContextString;
		FString RowName;
		RowName.AppendInt(_DialogueInfoIndex);
		if (_DialogueInfoIndex <= _Dialogue->GetRowNames().Num())
			return *_Dialogue->FindRow<FDialogueInfo>(FName(RowName), ContextString);
		else
			return NullDialogueInfo;
	}
	else
		return NullDialogueInfo;
}

void UDialogueSystem::Activate(APawn* Executor, UDataTable* Dialogue) {
	_Dialogue = Dialogue;
	_DialogueInfo = GetDialogueInfo();
	_Activator = Executor;
	if (_Activator->IsA(ACharacterBase::StaticClass())) {
		ACharacterBase* _Player = Cast<ACharacterBase>(_Activator);
		_Player->ActivateDialogueHUD();
		OnPlayerExecute();
	}
}