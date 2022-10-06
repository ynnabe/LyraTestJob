// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/LyraGameplayAbility_RangedWeapon.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "LGA_WeaponRocketLauncher.generated.h"


/**
 * 
 */
UCLASS()
class LYRAGAME_API ULGA_WeaponRocketLauncher : public ULyraGameplayAbility_RangedWeapon
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon parameters")
	UAnimMontage* CharacterFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon parameters")
	float AutoRate = 2.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon parameters")
	TSubclassOf<AActor> GrenadeLauncherClass;

	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* AbilityTask_PlayMontageAndWait;


private:

	UFUNCTION(Server, Reliable)
	void Server_LaunchRocket(FVector Location, FRotator Direction, APlayerController* LyraController);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LaunchRocket(FVector Location, FRotator Direction, APlayerController* LyraController);

	void GetDirectionAndLocation(const FGameplayAbilityActorInfo* ActorInfo, FRotator& Direction, FVector& Location);
	
	UFUNCTION()
	void EndFire();
};
