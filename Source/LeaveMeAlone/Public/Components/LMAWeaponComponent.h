// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAWeaponComponent.generated.h"

class ALMABaseWeapon;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAWeaponComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULMAWeaponComponent();

	void Fire();
	void StopFire();
	void Reload();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ALMABaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadMontage;

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ALMABaseWeapon* Weapon = nullptr;

	bool AnimReloading = false;

	void SpawnWeapon();
	bool CanReload() const;
	void ReloadIfEmptyOrPressed();

	void OnClipIsEmpty(bool Ammo);

	void InitAnimNotify();
	void OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh);
};
