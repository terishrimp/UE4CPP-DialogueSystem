// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "DialogueTest/Character/CharacterBase.h"
#include "Components/TextBlock.h"
#include "DialogueState.h"
#include "DialogueSystem.generated.h"

/**
 *
 */

USTRUCT(Blueprintable, BlueprintType)
struct FDialogueInfo : public FTableRowBase {
	GENERATED_BODY()

		FDialogueInfo() {
		Text = "NULL TEXT";
		Image = nullptr;
		TextSpeed = 0.f;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TextSpeed;
};

UENUM()
enum class EDialogueStat {
	Reading,
	Complete,
	Paused
};

UCLASS()
class DIALOGUETEST_API UDialogueSystem : public UObject
{
	GENERATED_BODY()

		friend class UDialogueState;
	friend class UReadingState;
	friend class UPauseState;
	friend class UCompleteState;
public:
	static const FDialogueInfo NullDialogueInfo;

	static UReadingState* _ReadingState;

	static UCompleteState* _CompleteState;

	static UPauseState* _PauseState;

	//void SetDialogue(UDataTable* Dialogue);

	UDialogueSystem();
	void Activate(APawn* Executor, UDataTable* Dialogue);
	FDialogueInfo GetDialogueInfo();
	void OnPlayerExecute();

protected:

	UPROPERTY()
		UDialogueState* _DialogueState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		UDataTable* _Dialogue;

	UPROPERTY()
		APawn* _Activator;

	//do not set freely, use Set
	UPROPERTY()
		EDialogueStat _Status;

	UPROPERTY()
		FTimerHandle _TextProgressHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float _TextSpeed;

	UPROPERTY()
		FDialogueInfo _DialogueInfo;

	UPROPERTY()
		uint8 _DialogueInfoIndex;

	UPROPERTY()
		FString _DisplayedText;

	UPROPERTY()
		uint8 _CurrentCharIndex;

	//Functions
	void SetStatus_Implementation(UDialogueState* const DialogueState);
};
