
#include "SExplosiveBarrel.h"

#include <Components/StaticMeshComponent.h>
#include <PhysicsEngine/RadialForceComponent.h>

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	

	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);
	ForceComp->SetAutoActivate(false); // Force component is doing some force in every tick by default.
	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2000.0f;
	ForceComp->bImpulseVelChange = true; // If true, it will ignore the mass of the objects.
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic); // By deault the constructor of force component already added the other channels.
}

void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Better to add the 
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();
}

void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

