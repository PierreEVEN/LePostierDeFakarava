// Pierre Even et Mathieu Voyer, tous droits reservés


#include "../../Public/Weapons/Canon.h"
#include "GameFramework/Pawn.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"
#include "Landscape/ProceduralLandscapeSection_Implementation.h"
#include "Kismet/KismetMathLibrary.h"

UCanon::UCanon(const class FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
	fireAudioComponent = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("fireAudioComponent"));
	fireAudioComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	SetComponentTickEnabled(true);

}

// Called when the game starts
void UCanon::BeginPlay()
{
	Super::BeginPlay();	
	fireAudioComponent->SetSound(fireAudio);
}


void UCanon::AddBullet()
{
	FBulletState newBullet;
	newBullet.position = GetComponentLocation();
	newBullet.velocity = GetForwardVector() * bulletVelocity + GetOwner()->GetRootComponent()->GetComponentVelocity();
	newBullet.lifeTime = bulletLifetime;
	if (APawn* owningPawn = Cast<APawn>(GetOwner()))
	{
		newBullet.owner = owningPawn->GetController();
	}
	newBullet.bulletParticle = NewObject<UParticleSystemComponent>(this);
	newBullet.bulletParticle->SetTemplate(bulletParticle);
	newBullet.bulletParticle->SetWorldLocation(newBullet.position);
	newBullet.bulletParticle->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(newBullet.velocity));
	newBullet.bulletParticle->RegisterComponent();

	if (canonParticle) UGameplayStatics::SpawnEmitterAttached(canonParticle, this, "", FVector(0), FRotator(0), EAttachLocation::SnapToTarget);

	bullets.Add(newBullet);
}

// Called every frame
void UCanon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LastShoot -= DeltaTime;

	if (bIsFiring && LastShoot < 0.f)
	{
		LastShoot = 1.f / FireRate;
		bulletLeft -= 1;
		AddBullet();
		if (bulletLeft <= 0)
		{
			StopFiring();
		}
	}

	for (int32 bulletIndex = bullets.Num() - 1; bulletIndex >= 0; --bulletIndex)
	{
		if (bullets[bulletIndex].lifeTime <= 0)
		{
			if (bullets[bulletIndex].bulletParticle) bullets[bulletIndex].bulletParticle->DestroyComponent();
			bullets.RemoveAt(bulletIndex);
		}
		else
		{
			FVector oldPosition = bullets[bulletIndex].position;


			bullets[bulletIndex].velocity += FVector(0, 0, -981 * DeltaTime);
			bullets[bulletIndex].velocity -= bullets[bulletIndex].velocity * bullets[bulletIndex].velocity * drag * DeltaTime;
			bullets[bulletIndex].position += bullets[bulletIndex].velocity * DeltaTime;
			bullets[bulletIndex].lifeTime -= DeltaTime;

			if (bullets[bulletIndex].bulletParticle) bullets[bulletIndex].bulletParticle->SetWorldLocation(bullets[bulletIndex].position);

			FHitResult result;
			if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), oldPosition, bullets[bulletIndex].position, ETraceTypeQuery::TraceTypeQuery1, true, { GetOwner() }, bDrawDebugTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, result, true))
			{
				FDamageEvent damageEvent;
				result.GetActor()->TakeDamage(bulletDamages, damageEvent, bullets[bulletIndex].owner, GetOwner());
				if (impactParticle)
				{
					UParticleSystemComponent* spawnedParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactParticle, FTransform(result.Location));
					spawnedParticle->SetWorldLocation(result.Location);

				}
				if (bullets[bulletIndex].bulletParticle) bullets[bulletIndex].bulletParticle->DestroyComponent();
				bullets.RemoveAt(bulletIndex);
				continue;
			}

			if (AProceduralLandscapeWrapper::GetLandscape())
			{
				float altitude = AProceduralLandscapeWrapper::GetLandscape()->GetAltitudeAtLocation(bullets[bulletIndex].position.X, bullets[bulletIndex].position.Y);
				if (altitude > bullets[bulletIndex].position.Z)
				{
					if (impactParticle)
					{
						UParticleSystemComponent* spawnedParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactParticle, FTransform(FVector(bullets[bulletIndex].position.X, bullets[bulletIndex].position.Y, altitude)));
						spawnedParticle->SetWorldLocation(FVector(bullets[bulletIndex].position.X, bullets[bulletIndex].position.Y, altitude));
					}
					if (bullets[bulletIndex].bulletParticle) bullets[bulletIndex].bulletParticle->DestroyComponent();
					bullets.RemoveAt(bulletIndex);
					continue;
				}
			}

		}
	}
}

void UCanon::StartFiring()
{
	if (bulletLeft > 0)
	{
		fireAudioComponent->Play();
		bIsFiring = true;
	}
}

void UCanon::StopFiring()
{
	fireAudioComponent->Stop();
	bIsFiring = false;
}