// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueSystem.h"

const FDialogueInfo UDialogueSystem::NullDialogueInfo = FDialogueInfo();

UReadingState* UDialogueSystem::_ReadingState;
UCompleteState* UDialogueSystem::_CompleteState;
UPauseState* UDialogueSystem::_PauseState;

UDialogueSystem::UDialogueSystem() {

	//AddToRoot is done so object referenced by static pointers is never destroyed
	if (!UDialogueSystem::_ReadingState) {
		UDialogueSystem::_ReadingState =CreateDefaultSubobject<UReadingState>(TEXT("ReadingState"));
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

EDialogueStat UDialogueSystem::SetStatus(const EDialogueStat Status) {
	if (_Status == Status) return _Status;
	//clear timer before next change
	GetWorld()->GetTimerManager().ClearTimer(_TextProgressHandle);

	switch (Status) {
	case EDialogueStat::Paused:
		//	display all text
		_DialogueInfoIndex++;
		_CurrentCharIndex = 0;
		DisplayAllChars();
		_Status = Status;
		break;
	case EDialogueStat::Reading:
		//	continue to next text
		_DialogueInfo = GetDialogueInfo();
		_TextSpeed = _DialogueInfo.TextSpeed;
		GetWorld()->GetTimerManager().SetTimer(_TextProgressHandle,
			this,
			&UDialogueSystem::IncrementChars,
			_TextSpeed,
			true,
			0.f);
		_Status = Status;
		break;
	case EDialogueStat::Complete:
		// exit widget
		if (_Activator->IsA(ACharacterBase::StaticClass())) {
			ACharacterBase* _Player = Cast<ACharacterBase>(_Activator);
			_Player->GetDialogueHUD()->DialogueText->SetText(FText::FromString(""));
			_Player->DeactivateDialogueHUD();
			_Player->bInDialogue = false;
		}
		_DialogueInfoIndex = 1;
		break;
	default:
		//should not occur since enum will always have a stat
		break;
	}
	return _Status;
}

void UDialogueSystem::SetStatus_Implementation(UDialogueState* const DialogueState)
{
	if(_DialogueState){
	_DialogueState = DialogueState;
	DialogueState->Enter(*this);
	}
}


void UDialogueSystem::OnPlayerExecute() {
	if(_DialogueState){
	_DialogueState = _DialogueState->HandleInput(*this);
	_DialogueState->Enter(*this);

	UE_LOG(LogTemp, Log, TEXT("%s"), *_DialogueState->GetName());
	}
	//if (_DialogueInfoIndex > _Dialogue->GetRowNames().Num())
	//{
	//	SetStatus(EDialogueStat::Complete);
	//}
	//else if (_Status == EDialogueStat::Reading) {
	//	SetStatus(EDialogueStat::Paused);
	//}
	//else if (_Status == EDialogueStat::Paused) {
	//	SetStatus(EDialogueStat::Reading);
	//}
	//else if (_Status == EDialogueStat::Complete) {
	//	SetStatus(EDialogueStat::Reading);
	//}

	//Get string from enum class
/*	const FString ResourceString = StaticEnum<EDialogueStat>()->GetValueAsString(_Status);
	UE_LOG(LogTemp, Log, TEXT("%s"), *ResourceString);
	UE_LOG(LogTemp, Log, TEXT("%d"), _DialogueInfoIndex)*/;
}

void UDialogueSystem::IncrementChars() {
	if (!_DialogueInfo.Text.IsEmpty() && _CurrentCharIndex < _DialogueInfo.Text.Len()) {
		_DisplayedText = _DialogueInfo.Text.Left(_CurrentCharIndex + 1);
		if (_Activator->IsA(ACharacterBase::StaticClass()))
			Cast<ACharacterBase>(_Activator)->GetDialogueHUD()->DialogueText->SetText(FText::FromString(_DisplayedText));
		_CurrentCharIndex++;
	}
	else {
		//status is paused
		SetStatus(EDialogueStat::Paused);
	}
}

void UDialogueSystem::DisplayAllChars() {
	if (_DisplayedText != _DialogueInfo.Text) {
		_DisplayedText = _DialogueInfo.Text;
		if (_Activator->IsA(ACharacterBase::StaticClass()))
			Cast<ACharacterBase>(_Activator)->GetDialogueHUD()->DialogueText->SetText(FText::FromString(_DisplayedText));
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