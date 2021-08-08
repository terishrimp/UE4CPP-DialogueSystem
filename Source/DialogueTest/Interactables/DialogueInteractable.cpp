// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueInteractable.h"

ADialogueInteractable::ADialogueInteractable() {
	PrimaryActorTick.bCanEverTick = true;
}



void ADialogueInteractable::BeginPlay() {
	Super::BeginPlay();
	_Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	UE_LOG(LogTemp, Log, TEXT("%s"), *_Player->GetName());
	//Add function to player on button press delegate
	//
	if (Cast<UDialogueTestGameInstance>(GetGameInstance()))
		_DialogueSystem = Cast<UDialogueTestGameInstance>(GetGameInstance())->GetDialogueSystem();
}

void ADialogueInteractable::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


bool ADialogueInteractable::Execute(APawn* Executor) {
	if (_Player == Executor) {
		UE_LOG(LogTemp, Log, TEXT("%s is executing"), *GetName());
		if (!_Player->bInDialogue) {
			_Player->bInDialogue = true;
			_DialogueSystem->Activate(Executor, _Dialogue);
		}
		else
		{
			_DialogueSystem->OnPlayerExecute();
		}
		return true;
	}
	return false;
}

