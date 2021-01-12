#include "HealthComponent.h"
#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;
	TankGameMode = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	UE_LOG(LogTemp, Warning, TEXT("health %f"), Health);

	Owner = GetOwner();

	if (Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("bind %s"), *GetOwner()->GetName());
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
	
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
					AController* InstigatedBy, AActor* DamageCauser) 
{
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage function called"));
	if (Damage == 0 || Health <= 0)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Damage taken = %f"), Damage);

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	if (Health <= 0)
	{
		if (TankGameMode && Owner)
		{
			TankGameMode->ActorDied(Owner);
		}
	}
}