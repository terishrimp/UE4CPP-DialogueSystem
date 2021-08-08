// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Interactable.generated.h"

UCLASS(Abstract)
class DIALOGUETEST_API AInteractable : public AActor
{
	GENERATED_BODY()

private:

	TArray<UMaterialInstanceDynamic*> DynamicMaterials;
	bool bIsSelected = false;

public:	
	// Sets default values for this actor's properties
	AInteractable();
	// Called every frame

	void SetIsSelected(bool _bIsSelected);
	virtual bool Execute(APawn* Executor) PURE_VIRTUAL(AInteractable::Execute, return false;);
protected:
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
