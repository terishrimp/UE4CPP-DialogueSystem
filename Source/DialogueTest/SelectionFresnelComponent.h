// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Array.h"
#include "Engine/StaticMesh.h"
#include "SelectionFresnelComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIALOGUETEST_API USelectionFresnelComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	APawn* PlayerPawn = nullptr;
	UStaticMeshComponent* StaticMesh;
	//TArray <UMaterialInterface> OriginalStaticMeshMaterials;
	//TArray<UMaterialInterface> FresnelStaticMeshMaterials;
	bool bIsSelected = false;
public:	
	// Sets default values for this component's properties
	USelectionFresnelComponent();

	//void SwapToFresnelMaterial();
	void SwapToOriginalMaterial();

	void SetIsSelected(bool Value);

	UFUNCTION()
	void OnComponentHit (UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
