// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionFresnelComponent.h"

// Sets default values for this component's properties
USelectionFresnelComponent::USelectionFresnelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// ...
}


// Called when the game starts
void USelectionFresnelComponent::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	PlayerPawn = GetOwner()->GetWorld()->GetFirstPlayerController()->GetPawn();
	//for (auto Material : StaticMesh->GetStaticMesh()->StaticMaterials) {
	//	OriginalStaticMeshMaterials.Emplace(*Material.MaterialInterface);
	//	FresnelStaticMeshMaterials.Emplace(*Material.MaterialInterface);
	//}

	StaticMesh->OnComponentHit.AddUniqueDynamic(this, &USelectionFresnelComponent::OnComponentHit);
	// ...

}

void USelectionFresnelComponent::SetIsSelected(bool Value) {
	bIsSelected = Value;
}

void USelectionFresnelComponent::OnComponentHit(
	UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit
	)
{
	//if (OtherActor && OtherActor == Cast<AActor>(PlayerPawn)) {
	//	SwapToOriginalMaterial();
		UE_LOG(LogTemp, Warning, TEXT("OVERLAP ENDED"));
	/*}*/
}

void USelectionFresnelComponent::SwapToOriginalMaterial() {
	return;
}
// Called every frame

