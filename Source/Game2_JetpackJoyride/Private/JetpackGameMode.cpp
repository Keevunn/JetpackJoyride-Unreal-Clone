// Fill out your copyright notice in the Description page of Project Settings.


#include "JetpackGameMode.h"

#include "EngineUtils.h"
#include "JetpackPlayer.h"
#include "LevelChunk.h"
#include "Kismet/GameplayStatics.h"

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
	
	if (!ActiveChunks.IsEmpty()) 
		CorridorEndX = ActiveChunks.Last()->GetActorLocation().X;
}

void AJetpackGameMode::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (!IsValid(Player)) Player = Cast<AJetpackPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!IsValid(Player) || ActiveChunks.IsEmpty()) return;
	
	const FVector PlayerPosition = Player->GetActorLocation();
	
	if (CorridorEndX < PlayerPosition.X + ForwardGenerationDistance) SpawnNextChunk(); // Spawn new chunk
	
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
	
	float SpawnX = EndChunkX + EndChunkWidth/2.0f + DefaultChunk->GetChunkWidth()/2.0f;
	FVector SpawnLocation(SpawnX, 0, 0);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	ALevelChunk* SpawnChunk = GetWorld()->SpawnActor<ALevelChunk>(SpawnChunkClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	if (!IsValid(SpawnChunk)) return;
	
	ActiveChunks.Add(SpawnChunk);
	CorridorEndX = SpawnX;
}
