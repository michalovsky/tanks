#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"

void ATankGameModeBase::BeginPlay() 
{
    Super::BeginPlay();
    HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor) 
{
    if (PlayerTank && DeadActor == PlayerTank)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerTank died"));
        PlayerTank->HandleDestruction();
        HandleGameOver(false);

        if (PlayerController)
        {
            PlayerController->SetPlayerEnabledState(false);
        }
    }
    else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("Turret died"));
        DestroyedTurret->HandleDestruction();

        if (--TargetTurrets == 0)
        {
            HandleGameOver(true);
        }
    }
}

void ATankGameModeBase::HandleGameStart() 
{
    TArray<AActor*> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), TurretActors);
    TargetTurrets = TurretActors.Num();
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

    GameStart();

    if (PlayerController)
    {
        PlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableHandle;
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerController,
                                                &APlayerControllerBase::SetPlayerEnabledState, true);
        GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
    }
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon) 
{
    GameOver(PlayerWon);
}
