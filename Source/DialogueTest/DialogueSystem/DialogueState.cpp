// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueState.h"
#include "DialogueSystem.h"
//base
UDialogueState::UDialogueState()
{}

//reading
UReadingState::UReadingState()
{}

void UReadingState::Enter(UDialogueSystem& ds)
{
	_ds = &ds;
	TimerDelegate.BindUFunction(this, FName("IncrementChars"), &ds);
	ds._DialogueInfo = ds.GetDialogueInfo();
	ds._TextSpeed = ds._DialogueInfo.TextSpeed;
	ds.GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		TimerDelegate,
		ds._TextSpeed,
		true,
		0.f);
}

void UReadingState::Update(UDialogueSystem& ds)
{
}

void UReadingState::IncrementChars(UDialogueSystem* ds)
{
	if (ds) {
		if (!ds->_DialogueInfo.Text.IsEmpty() && ds->_CurrentCharIndex < ds->_DialogueInfo.Text.Len()) {
			ds->_DisplayedText = ds->_DialogueInfo.Text.Left(ds->_CurrentCharIndex + 1);
			if (ds->_Activator->IsA(ACharacterBase::StaticClass()))
				Cast<ACharacterBase>(ds->_Activator)->GetDialogueHUD()->DialogueText->SetText(FText::FromString(ds->_DisplayedText));
			ds->_CurrentCharIndex++;
		}
		else {
			//status is paused
			ds->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			ds->SetStatus_Implementation(UDialogueSystem::_PauseState);
		}
	}
}
UDialogueState* UReadingState::Exit(UDialogueSystem& ds)
{
	return nullptr;
}
UDialogueState* UReadingState::HandleInput(UDialogueSystem& ds)
{
	_ds = &ds;
	ds.GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return UDialogueSystem::_PauseState;
}

//complete
UCompleteState::UCompleteState()
{
}

void UCompleteState::Enter(UDialogueSystem& ds)
{
	if (ds._Activator->IsA(ACharacterBase::StaticClass()))
	{
		ACharacterBase* _Player = Cast<ACharacterBase>(ds._Activator);
		_Player->GetDialogueHUD()->DialogueText->SetText(FText::FromString(""));
		_Player->DeactivateDialogueHUD();
		_Player->bInDialogue = false;
	}
	ds._DialogueInfoIndex = 1;
}
void UCompleteState::Update(UDialogueSystem& ds)
{
}

UDialogueState* UCompleteState::Exit(UDialogueSystem& ds)
{
	return nullptr;
}
UDialogueState* UCompleteState::HandleInput(UDialogueSystem& ds)
{
	return UDialogueSystem::_ReadingState;
}

//pause
UPauseState::UPauseState()
{
}

void UPauseState::Enter(UDialogueSystem& ds)
{
	ds._DialogueInfoIndex++;
	ds._CurrentCharIndex = 0;
	DisplayAllChars(ds);
}
void UPauseState::Update(UDialogueSystem& ds)
{
}

UDialogueState* UPauseState::Exit(UDialogueSystem& ds)
{
	return nullptr;
}
UDialogueState* UPauseState::HandleInput(UDialogueSystem& ds)
{
	if (ds._DialogueInfoIndex > ds._Dialogue->GetRowNames().Num())
		return UDialogueSystem::_CompleteState;
	else
		return UDialogueSystem::_ReadingState;
}

void UPauseState::DisplayAllChars(UDialogueSystem& ds)
{
	if (ds._DisplayedText != ds._DialogueInfo.Text) {
		ds._DisplayedText = ds._DialogueInfo.Text;
		if (ds._Activator->IsA(ACharacterBase::StaticClass()))
			Cast<ACharacterBase>(ds._Activator)->GetDialogueHUD()->DialogueText->SetText(FText::FromString(ds._DisplayedText));
	}
}