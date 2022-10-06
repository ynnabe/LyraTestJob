// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/LyraGameplayAbility_Sprint.h"
#include "Character/LyraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void ULyraGameplayAbility_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if(!HasAuthority(&ActivationInfo))
	{
		return;
	}

	ALyraCharacter* LyraCharacter = StaticCast<ALyraCharacter*>(ActorInfo->AvatarActor.Get());
	LyraCharacter->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Sprint pressed")));
	AbilityTask_WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
	AbilityTask_WaitInputPress->OnPress.AddDynamic(this, &ULyraGameplayAbility_Sprint::EndSprint);
	AbilityTask_WaitInputPress->Activate();
}

void ULyraGameplayAbility_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if(!HasAuthority(&ActivationInfo))
	{
		return;
	}
	
	ALyraCharacter* LyraCharacter = StaticCast<ALyraCharacter*>(ActorInfo->AvatarActor.Get());
	LyraCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULyraGameplayAbility_Sprint::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	ALyraCharacter* LyraCharacter = StaticCast<ALyraCharacter*>(ActorInfo->AvatarActor.Get());
	LyraCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void ULyraGameplayAbility_Sprint::EndSprint(float TimeHeld)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Sprint released")));
	K2_EndAbility();
}
