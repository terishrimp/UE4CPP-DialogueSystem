// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "DialogueTest/Interactables/Interactable.h"
#include "Blueprint/UserWidget.h"
#include "DialogueTest/CharacterHUD.h"
#include "CharacterBase.generated.h"

UCLASS()
class DIALOGUETEST_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = DialogueOptions, meta = (AllowPrivateAccess = true))
		TSubclassOf<UCharacterHUD> _DialogueHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = DialogueOptions, meta = (AllowPrivateAccess = true))
		TSubclassOf<UUserWidget> _InteractHintHUDClass;

	UPROPERTY()
		UCharacterHUD* _DialogueHUD;

	UPROPERTY()
		UUserWidget* _InteractHintHUD;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Jump() override;

	void TurnYaw(float Rate);

	void TurnPitch(float Rate);

	void Interact();

	void SetIgnoreControllerInput(const bool bIgnoreInput = true) const;

	void DisplayDebugRay(const bool bHitSomething);

	void GetPlayerRayPoints(FVector* out_StartPos, FVector* out_EndPos);

	void GetRaycastResults(FHitResult& out_HitResult, bool& out_bTraceHasHit);

	FHitResult HitInfo;

	FHitResult PreviousHitInfo;

	bool bTraceHasHit;

	UPROPERTY(EditAnywhere, Category = InteractionOptions)
		float RayLength = 200;
public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCharacterHUD* GetDialogueHUD();
	void ActivateDialogueHUD();
	void DeactivateDialogueHUD();

	bool bInDialogue = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
