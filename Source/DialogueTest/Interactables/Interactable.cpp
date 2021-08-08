// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

// Sets default values
AInteractable::AInteractable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = StaticMesh;

}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	if (StaticMesh->GetMaterials().Num() > 0) {
		for (size_t i{ 0 }; i < StaticMesh->GetMaterials().Num(); i++) {
			DynamicMaterials.Emplace(StaticMesh->CreateAndSetMaterialInstanceDynamic(i));
		}
	}
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AInteractable::SetIsSelected(bool _bIsSelected) {
	if (bIsSelected == _bIsSelected) { return; }
	bIsSelected = _bIsSelected;
	if (DynamicMaterials.Num() > 0)
	{
		if (bIsSelected)
		{
			for (auto Material : DynamicMaterials) {
				Material->SetScalarParameterValue("FresnelToggle", 1);
			}
		}
		else
		{
			for (auto Material : DynamicMaterials) {
				Material->SetScalarParameterValue("FresnelToggle", 0);
			}
		}
	}
}