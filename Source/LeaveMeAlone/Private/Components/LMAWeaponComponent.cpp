// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Components/LMAWeaponComponent.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/LMABaseWeapon.h"

// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULMAWeaponComponent::BeginPlay() {
	Super::BeginPlay();

	SpawnWeapon();
	InitAnimNotify();
}

// Called every frame
void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULMAWeaponComponent::Fire() {
	if (Weapon && !AnimReloading) {
		Weapon->Fire();
	}
}
void ULMAWeaponComponent::StopFire() {
	if (Weapon) {
		Weapon->StopFire();
	}
}
void ULMAWeaponComponent::SpawnWeapon() {
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon) {
		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character) {
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");
			Weapon->OnClipIsEmpty.AddUObject(this, &ULMAWeaponComponent::OnClipIsEmpty);
		}
	}
}
bool ULMAWeaponComponent::CanReload() const { return !AnimReloading && !Weapon->IsCurrentClipFull(); }
void ULMAWeaponComponent::Reload() { ReloadIfEmptyOrPressed(); }

void ULMAWeaponComponent::ReloadIfEmptyOrPressed() {
	if (!CanReload()) return;
	AnimReloading = true;
	StopFire();
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(ReloadMontage);
	Weapon->ChangeClip();
}

void ULMAWeaponComponent::OnClipIsEmpty(bool Ammo) {
	if (Ammo) {
		ReloadIfEmptyOrPressed();
		GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Green, FString::Printf(TEXT("Reload?: %i"), Ammo));
	} else 
		GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Red, FString::Printf(TEXT("Reload?: %i"), Ammo));
}

void ULMAWeaponComponent::InitAnimNotify() {
	if (!ReloadMontage) return;
	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents) {
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish) {
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}
void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh) {
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh) {
		AnimReloading = false;
	}
}

bool ULMAWeaponComponent::GetCurrentWeaponAmmo(FAmmoWeapon& AmmoWeapon) const {
	if (Weapon) {
		AmmoWeapon = Weapon->GetCurrentAmmoWeapon();
		return true;
	}
	return false;
}