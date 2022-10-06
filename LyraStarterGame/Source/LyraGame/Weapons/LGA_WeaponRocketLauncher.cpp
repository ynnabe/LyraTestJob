// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/LGA_WeaponRocketLauncher.h"

#include "Character/LyraCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/LyraPlayerController.h"

void ULGA_WeaponRocketLauncher::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(IsLocallyControlled())
	{
		StartRangedWeaponTargeting();
	}

	AbilityTask_PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CharacterFireMontage, AutoRate, NAME_None, false, 1.0, 0.0f);
	AbilityTask_PlayMontageAndWait->OnCompleted.AddDynamic(this, &ULGA_WeaponRocketLauncher::EndFire);
	AbilityTask_PlayMontageAndWait->OnInterrupted.AddDynamic(this, &ULGA_WeaponRocketLauncher::EndFire);
	AbilityTask_PlayMontageAndWait->OnCancelled.AddDynamic(this, &ULGA_WeaponRocketLauncher::EndFire);
	AbilityTask_PlayMontageAndWait->Activate();
	
	FVector Location;
	FRotator Direction;
	GetDirectionAndLocation(ActorInfo, Direction, Location);

	if(IsLocallyControlled())
	{
		Server_LaunchRocket(Location, Direction, ActorInfo->PlayerController.Get());
	}
	
}

void ULGA_WeaponRocketLauncher::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULGA_WeaponRocketLauncher::Server_LaunchRocket_Implementation(FVector Location, FRotator Direction, APlayerController* LyraController)
{
	Multicast_LaunchRocket(Location, Direction, LyraController);
}

void ULGA_WeaponRocketLauncher::Multicast_LaunchRocket_Implementation(FVector Location, FRotator Direction, APlayerController* LyraController)
{
	if(IsValid(LyraController))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = LyraController->GetPawn();
		GetWorld()->SpawnActor<AActor>(GrenadeLauncherClass, Location, Direction, SpawnParameters);
	}
}

void ULGA_WeaponRocketLauncher::GetDirectionAndLocation(const FGameplayAbilityActorInfo* ActorInfo, FRotator& Direction, FVector& Location)
{
	const ALyraPlayerController* PlayerController = StaticCast<ALyraPlayerController*>(ActorInfo->PlayerController.Get());
	const APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager;

	const ALyraCharacter* LyraCharacter = StaticCast<ALyraCharacter*>(ActorInfo->AvatarActor.Get());
	Location = LyraCharacter->GetActorForwardVector() * 350.0f + PlayerCameraManager->GetCameraLocation();
	
	const FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	const FVector EndLocation = (PlayerCameraManager->GetActorForwardVector() * 5000.0f) + StartLocation;

	FHitResult HitResult;
	if(bool bHasHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility))
	{
		Direction = UKismetMathLibrary::FindLookAtRotation(StartLocation, HitResult.Location);
	}
	Direction = UKismetMathLibrary::FindLookAtRotation(StartLocation, HitResult.TraceEnd);
}

void ULGA_WeaponRocketLauncher::EndFire()
{
	K2_EndAbility();
}
