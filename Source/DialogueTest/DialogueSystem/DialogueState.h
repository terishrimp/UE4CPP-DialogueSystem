// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueState.generated.h"
/**
 *
 */
class UDialogueSystem;
UCLASS(Abstract, Blueprintable)
class DIALOGUETEST_API UDialogueState : public UObject
{
	GENERATED_BODY()
public:

	UDialogueState();
	virtual void Enter(UDialogueSystem& ds) PURE_VIRTUAL(&UDialogueState::Enter, return;);
	virtual void Update(UDialogueSystem& ds) PURE_VIRTUAL(&UDialogueState::Update, return;);
	virtual UDialogueState* Exit(UDialogueSystem& ds) PURE_VIRTUAL(&UDialogueState::Exit, return nullptr;);
	virtual UDialogueState* HandleInput(UDialogueSystem& ds) PURE_VIRTUAL(&UDialogueState::HandleInput, return nullptr;);

private:
	void OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS);
};

UCLASS(Blueprintable)
class DIALOGUETEST_API UReadingState : public UDialogueState
{
	GENERATED_BODY()
public:
	UReadingState();
	virtual void Enter(UDialogueSystem& ds) override;
	virtual void Update(UDialogueSystem& ds) override;
	virtual UDialogueState* Exit(UDialogueSystem& ds) override;
	virtual UDialogueState* HandleInput(UDialogueSystem& ds) override;

	UFUNCTION()
	void IncrementChars(UDialogueSystem* ds);

private:
	UPROPERTY()
	UDialogueSystem* _ds;

	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;
};

UCLASS(Blueprintable)
class DIALOGUETEST_API UCompleteState : public UDialogueState
{
	GENERATED_BODY()
public:
	UCompleteState();
	virtual void Enter(UDialogueSystem& ds) override;
	virtual void Update(UDialogueSystem& ds) override;
	virtual UDialogueState* Exit(UDialogueSystem& ds) override;
	virtual UDialogueState* HandleInput(UDialogueSystem& ds) override;

};

UCLASS(Blueprintable)
class DIALOGUETEST_API UPauseState : public UDialogueState
{
	GENERATED_BODY()
public:
	UPauseState();
	virtual void Enter(UDialogueSystem& ds) override;
	virtual void Update(UDialogueSystem& ds) override;
	virtual UDialogueState* Exit(UDialogueSystem& ds) override;
	virtual UDialogueState* HandleInput(UDialogueSystem& ds) override;

};

UCLASS(Blueprintable)
class DIALOGUETEST_API UNullDialogueState : public UDialogueState
{
	GENERATED_BODY()
public:
	UNullDialogueState() {};
	virtual void Enter(UDialogueSystem& ds) override {};
	virtual void Update(UDialogueSystem& ds) override {};
	virtual UDialogueState* Exit(UDialogueSystem& ds) override { return nullptr; };
	virtual UDialogueState* HandleInput(UDialogueSystem& ds) override { return nullptr; };
};

