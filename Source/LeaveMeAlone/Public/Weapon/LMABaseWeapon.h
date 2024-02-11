// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipIsEmpty, bool)

class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct FAmmoWeapon {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite;
};

UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALMABaseWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();
	void StopFire();
	void ChangeClip();
	bool IsCurrentClipFull() const;

	FOnClipIsEmpty OnClipIsEmpty;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon AmmoWeapon{ 30, 0, true };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceDistance = 800.0f;


	void Shoot();
	void DecrementBullets();
	bool IsCurrentClipEmpty() const;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FAmmoWeapon CurrentAmmoWeapon;
	FTimerHandle TimerHandle;
};
