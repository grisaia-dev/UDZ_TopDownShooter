// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMAEnemyCharacter.generated.h"

class ULMAHealthComponent;
class UBehaviorTree;

UCLASS()
class LEAVEMEALONE_API ALMAEnemyCharacter : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMAEnemyCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI") UBehaviorTree* BehaviorTreeAsset;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components") ULMAHealthComponent* HealthComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
