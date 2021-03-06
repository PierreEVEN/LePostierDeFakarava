// Pierre Even et Mathieu Voyer, tous droits reservés

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Canon.generated.h"

class UParticleSystem;
class USoundBase;
class AController;
class UParticleSystemComponent;

USTRUCT(BlueprintType)
struct LEPOSTIERDEFAKARAVA_API FBulletState
{
	GENERATED_BODY()
public:

	UPROPERTY()
	float lifeTime = 0.f;

	UPROPERTY()
		FVector position;

	UPROPERTY()
		FVector velocity;

	UPROPERTY()
		AController* owner;

	UPROPERTY()
		UParticleSystemComponent* bulletParticle;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEPOSTIERDEFAKARAVA_API UCanon : public USceneComponent
{
	GENERATED_BODY()

public:	
	UCanon(const class FObjectInitializer& ObjectInitializer);

private:

	UFUNCTION()
	void OnRep_IsFIring() {}

protected:

	UPROPERTY()
	bool bIsFiring = false;

	UPROPERTY()
		float LastShoot = 0.f;

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
		TArray<FBulletState> bullets;

	UPROPERTY()
		UAudioComponent* fireAudioComponent = nullptr;
public:	

	void AddBullet();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* bulletParticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* canonParticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* impactParticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* fireAudio = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FireRate = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bulletVelocity = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bulletLifetime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float drag = 0.01f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 maxBullets = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 bulletLeft = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDrawDebugTraces = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bulletDamages = 100.f;

	UFUNCTION(BlueprintCallable)
		void StartFiring();

	UFUNCTION(BlueprintCallable)
		void StopFiring();


};
