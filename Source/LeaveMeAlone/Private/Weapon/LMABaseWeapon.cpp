// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Weapon/LMABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/TimerHandle.h"

//DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

// Sets default values
ALMABaseWeapon::ALMABaseWeapon() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);
	
}

// Called when the game starts or when spawned
void ALMABaseWeapon::BeginPlay() {
	Super::BeginPlay();
	CurrentAmmoWeapon = AmmoWeapon;
}

// Called every frame
void ALMABaseWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ALMABaseWeapon::Fire() {
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ALMABaseWeapon::Shoot, 0.2f, true, 0.0f);
}
void ALMABaseWeapon::StopFire() {
	GetWorldTimerManager().ClearTimer(TimerHandle);
}
void ALMABaseWeapon::Shoot() {
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit) {
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
	}
	DecrementBullets();
}

void ALMABaseWeapon::ChangeClip() { CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets; }
bool ALMABaseWeapon::IsCurrentClipEmpty() const { return CurrentAmmoWeapon.Bullets == 0; }
bool ALMABaseWeapon::IsCurrentClipFull() const { return CurrentAmmoWeapon.Bullets == 30; }
void ALMABaseWeapon::DecrementBullets() {
	CurrentAmmoWeapon.Bullets = FMath::Clamp(CurrentAmmoWeapon.Bullets - 1, 0, 30);
	GEngine->AddOnScreenDebugMessage(4, 2.0f, FColor::Green, FString::Printf(TEXT("Reload?: %i"), CurrentAmmoWeapon.Bullets));
	//UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));
	OnClipIsEmpty.Broadcast(IsCurrentClipEmpty());
}
