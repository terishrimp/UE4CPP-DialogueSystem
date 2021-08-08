// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#define OUT
// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	_DialogueHUD = Cast<UCharacterHUD>(CreateWidget<UUserWidget>(Cast<APlayerController>(Controller), _DialogueHUDClass));
	GetCharacterMovement()->AirControl = 1;
	_InteractHintHUD = CreateWidget<UUserWidget>(Cast<APlayerController>(Controller), _InteractHintHUDClass);
}

void ACharacterBase::DisplayDebugRay(const bool bHitSomething) {
	FColor DebugLineColor = FColor::Red;

	FVector RayStartPos;
	FVector RayEndPos;
	GetPlayerRayPoints(&RayStartPos, &RayEndPos);

	if (bHitSomething) {
		DebugLineColor = FColor::Green;
	}

	DrawDebugLine(
		GetWorld(),
		RayStartPos,
		RayEndPos,
		DebugLineColor,
		false,
		0.2f,
		0,
		1);
}

void ACharacterBase::GetPlayerRayPoints(FVector* out_StartPos, FVector* out_EndPos)
{
	FVector TraceStartPos;
	FRotator ActorViewDirection;
	GetActorEyesViewPoint(OUT TraceStartPos, OUT ActorViewDirection);
	if (out_StartPos)
	{
		*out_StartPos = TraceStartPos;
	}
	if (out_EndPos) {
		*out_EndPos = TraceStartPos + ActorViewDirection.Vector() * RayLength;
	}
}

void ACharacterBase::GetRaycastResults(FHitResult& out_HitResult, bool& out_bTraceHasHit) {
	FVector RayStartPos;
	FVector RayEndPos;
	GetPlayerRayPoints(&RayStartPos, &RayEndPos);
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, this);
	out_bTraceHasHit = GetWorld()->LineTraceSingleByChannel(
		OUT out_HitResult,
		RayStartPos,
		RayEndPos,
		ECollisionChannel::ECC_GameTraceChannel1,
		TraceParams
	);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetRaycastResults(OUT HitInfo, OUT bTraceHasHit);
	//DisplayDebugRay(bTraceHasHit);

	if (HitInfo.GetActor() && HitInfo.GetActor()->IsA(AInteractable::StaticClass()))
	{
		Cast<AInteractable>(HitInfo.GetActor())->SetIsSelected(true);
		if (_InteractHintHUD )
		{
			if (!bInDialogue && !_InteractHintHUD->IsInViewport())
				_InteractHintHUD->AddToViewport();
			else if (bInDialogue && _InteractHintHUD->IsInViewport())
				_InteractHintHUD->RemoveFromViewport();

		}

	}
	else if (PreviousHitInfo.GetActor() != HitInfo.GetActor() &&
		PreviousHitInfo.GetActor() &&
		PreviousHitInfo.GetActor()->IsA(AInteractable::StaticClass()))
	{
		Cast<AInteractable>(PreviousHitInfo.GetActor())->SetIsSelected(false);
		if (_InteractHintHUD && _InteractHintHUD->IsInViewport())
			_InteractHintHUD->RemoveFromViewport();
	}

	PreviousHitInfo = HitInfo;

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACharacterBase::Interact);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::Jump);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACharacterBase::TurnYaw);
	PlayerInputComponent->BindAxis("Lookup", this, &ACharacterBase::TurnPitch);
}


UCharacterHUD* ACharacterBase::GetDialogueHUD() {
	return _DialogueHUD;
}

void ACharacterBase::SetIgnoreControllerInput(const bool bIgnoreInput) const 
{
	Controller->SetIgnoreMoveInput(bIgnoreInput);
	Controller->SetIgnoreLookInput(bIgnoreInput);
}

void ACharacterBase::ActivateDialogueHUD() {
	SetIgnoreControllerInput(true);
	FInputModeGameAndUI Mode;
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	Mode.SetHideCursorDuringCapture(true);
	Cast<APlayerController>(GetController())->SetInputMode(Mode);
	_DialogueHUD->AddToViewport();
}


void ACharacterBase::DeactivateDialogueHUD() {
	SetIgnoreControllerInput(false);
	FInputModeGameOnly Mode;
	Mode.SetConsumeCaptureMouseDown(false);
	Cast<APlayerController>(GetController())->SetInputMode(Mode);
	_DialogueHUD->RemoveFromViewport();
}

void ACharacterBase::Interact() {
	if (HitInfo.GetActor() && HitInfo.GetActor()->IsA(AInteractable::StaticClass())) {
		Cast<AInteractable>(HitInfo.GetActor())->Execute(this);
	}
}

void ACharacterBase::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ACharacterBase::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ACharacterBase::Jump() {
	if(!Controller->IsMoveInputIgnored())
		Super::Jump();
}

void ACharacterBase::TurnYaw(float Rate) {
	APawn::AddControllerYawInput(Rate);
}

void ACharacterBase::TurnPitch(float Rate) {
	APawn::AddControllerPitchInput(Rate);
}
