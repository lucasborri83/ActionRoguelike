// Fill out your copyright notice in the Description page of Project Settings.

#include "SDashProjectile.h"
#include "Gameframework/ProjectileMovementComponent.h"

#include <Particles/ParticleSystemComponent.h>
#include <Kismet/GameplayStatics.h>

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDashProjectile::Explode, DetonateDelay);
}

void ASDashProjectile::Explode_Implementation()
{
	// Clear timer if the Explode was already called through another source like OnActorHit.
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	// Fist we need to stop the movement, stop the particle effect and disable the projectile collision
	EffectComp->DeactivateSystem();
	MovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);

	// Then we need to emit the explosion VFX
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	// Last, we initiate the timer for tele-porting to the instigator.
	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);
}

void ASDashProjectile::TeleportInstigator()
{
	// Get the actor that launched the projectile and then teleport it to the projectile location/rotation.
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		// Instead of just setting a new location/rotation, we are using teleport to, which checks many things (like the space were
		// we are going to teleport to) before allowing us to move the actor.
		ActorToTeleport->TeleportTo(GetActorLocation(), GetActorRotation(), false, false);
	}
}

ASDashProjectile::ASDashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MovementComponent->InitialSpeed = 6000.0f;
}
