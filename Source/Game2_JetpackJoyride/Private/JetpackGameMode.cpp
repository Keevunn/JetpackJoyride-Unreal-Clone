// Fill out your copyright notice in the Description page of Project Settings.


#include "JetpackGameMode.h"

#include "EngineUtils.h"
#include "JetpackPlayer.h"
#include "Laser.h"
#include "LevelChunk.h"
#include "Kismet/GameplayStatics.h"

FLaserSpawnSettings LaserPresets::Generate() {
	FLaserSpawnSettings Settings;
	
	Settings.Length = Lengths[FMath::RandRange(0, UE_ARRAY_COUNT(Lengths) - 1)];
	Settings.AngleDeg = Angles[FMath::RandRange(0, UE_ARRAY_COUNT(Angles) - 1)];	
	
	float AngleRadians = FMath::DegreesToRadians(Settings.AngleDeg);
	float HalfHeight = Settings.Length * 0.5f * FMath::Abs(FMath::Cos(AngleRadians));
	
	float MaxZ = BoundaryValue - HalfHeight;
	float MinZ = (-1.f * BoundaryValue) + HalfHeight;
	if (MinZ > MaxZ) return FLaserSpawnSettings(); // Won't fit so spawns at origin, 0 length
	
	Settings.ZPosition = FMath::FRandRange(MinZ, MaxZ);
	
	return Settings;
}

AJetpackGameMode::AJetpackGameMode() {
	PrimaryActorTick.bCanEverTick = true;
	
	ForwardGenerationDistance = (VisibleWidth / 2) + LargestChunkWidth;
}

void AJetpackGameMode::BeginPlay() {
	Super::BeginPlay();
	
	for (TActorIterator<ALevelChunk> It(GetWorld()); It; ++It) 
		ActiveChunks.Add(*It);
	
	ActiveChunks.Sort([](const ALevelChunk& A, const ALevelChunk& B) {
		return A.GetActorLocation().X < B.GetActorLocation().X;
	});
	
	if (ActiveChunks.IsEmpty()) return;
	
	CorridorEndX = ActiveChunks.Last()->GetActorLocation().X;
	
	const TObjectPtr<ALevelChunk>& EndChunk = ActiveChunks.Last();
	NextLaserStartX = CorridorEndX + EndChunk->GetChunkWidth() * 0.5f;
	LastLaserEndX = NextLaserStartX;
}

void AJetpackGameMode::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (!IsValid(Player)) Player = Cast<AJetpackPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!IsValid(Player) || ActiveChunks.IsEmpty()) return;
	
	const FVector PlayerPosition = Player->GetActorLocation();
	
	if (CorridorEndX < PlayerPosition.X + ForwardGenerationDistance) SpawnNextChunk(); // Spawn new chunk
	
	if (ActiveLasers.Num() < MaxLasers) {
		float SpawnLaserP = FMath::FRandRange(0.f, 1.f);
		float SpawnGapP = FMath::FRandRange(0.f, 1.f);
		if (SpawnLaserP <= LaserSpawnFrequency * DeltaSeconds) {
			SpawnLaser();
			UE_LOG(LogTemp, Warning, TEXT("Laser Spawned! | Next Laser Start: %f | Last Laser End: %f"), NextLaserStartX, LastLaserEndX);
		}
		else { // TODO limit by player's speed
			NextLaserStartX = LastLaserEndX + FMath::FRandRange(MinGap, MaxGap);
			LastLaserEndX = NextLaserStartX;
			UE_LOG(LogTemp, Warning, TEXT("Gap! | Next Laser Start: %f | Last Laser End: %f"), NextLaserStartX, LastLaserEndX);
		}
	}
	
	while (ActiveChunks.Num() > 1) {
		ALevelChunk* OldestChunk = ActiveChunks[0].Get();
		if (!IsValid(OldestChunk)) {
			ActiveChunks.RemoveAt(0);
			continue;
		}
		
		// If oldest chunk in visible range break loop
		if (OldestChunk->GetActorLocation().X >= PlayerPosition.X - BackGenerationDistance) break;
		
		OldestChunk->Destroy();
		ActiveChunks.RemoveAt(0);
	}
	
	while (ActiveLasers.Num() > 1) {
		ALaser* OldestLaser = ActiveLasers[0].Get();
		if (!IsValid(OldestLaser)) {
			ActiveLasers.RemoveAt(0);
			continue;
		}
		
		if (OldestLaser->GetActorLocation().X >= PlayerPosition.X - BackGenerationDistance) break;
		OldestLaser->Destroy();
		ActiveLasers.RemoveAt(0);
	}
}

void AJetpackGameMode::SpawnNextChunk() {
	if (LevelChunkPool.IsEmpty() || ActiveChunks.IsEmpty()) return;
	int SpawnIndex = FMath::RandRange(0, LevelChunkPool.Num() - 1);
	TSubclassOf<ALevelChunk> SpawnChunkClass = LevelChunkPool[SpawnIndex];
	if (!SpawnChunkClass) return;
	const ALevelChunk* DefaultChunk = SpawnChunkClass.GetDefaultObject();
	
	TObjectPtr<ALevelChunk> EndChunk = ActiveChunks.Last();
	float EndChunkX = EndChunk->GetActorLocation().X;
	float EndChunkWidth = EndChunk->GetChunkWidth();
	
	float SpawnX = EndChunkX + EndChunkWidth*0.5f + DefaultChunk->GetChunkWidth()*0.5f;
	FVector SpawnLocation(SpawnX, 0, 0);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	ALevelChunk* SpawnChunk = GetWorld()->SpawnActor<ALevelChunk>(SpawnChunkClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	if (!IsValid(SpawnChunk)) return;
	
	ActiveChunks.Add(SpawnChunk);
	CorridorEndX = SpawnX;
}

void AJetpackGameMode::SpawnLaser() {
	const FLaserSpawnSettings Settings = LaserPresets::Generate();
	float AngleRadians = FMath::DegreesToRadians(Settings.AngleDeg);
	float HalfWidth = Settings.Length * 0.5f * FMath::Abs(FMath::Sin(AngleRadians));
	
	if (!LaserClass) return;
	
	float SpawnX = NextLaserStartX + HalfWidth;
	const FVector SpawnLocation(SpawnX, 0, Settings.ZPosition);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ALaser* Laser = GetWorld()->SpawnActor<ALaser>(LaserClass, SpawnLocation, FRotator(Settings.AngleDeg, 0.f, 0.f), SpawnParams);
	Laser->SetLength(Settings.Length);
	Laser->SetRotation(Settings.AngleDeg);
	if (!IsValid(Laser)) return;
	
	ActiveLasers.Add(Laser);
	LastLaserEndX = SpawnX + HalfWidth;
	NextLaserStartX = LastLaserEndX + FMath::FRandRange(MinGap, MaxGap);
}
