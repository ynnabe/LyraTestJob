// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "LyraGameplayAbility_Sprint.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraGameplayAbility_Sprint : public ULyraGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Sprint parameters")
	float SprintSpeed = 2000.0f;

	void SetDefaultWalkSpeed(const FGameplayAbilityActorInfo* ActorInfo);

	void WaitInputPress();

	UFUNCTION()
	void EndSprint(float TimeHeld);

	UPROPERTY()
	UAbilityTask_WaitInputPress* AbilityTask_WaitInputPress;
};
