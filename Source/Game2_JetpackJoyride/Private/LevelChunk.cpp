// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChunk.h"

#include "JetpackGameMode.h"
#include "JetpackPlayer.h"
#include "PaperSpriteComponent.h"
#include "PaperTileMapComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ALevelChunk::ALevelChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(SceneRoot);

	BackgroundSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundSprite"));
	BackgroundSprite->SetupAttachment(SceneRoot);
	
	ForegroundTileMap = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("ForegroundTileMap"));
	ForegroundTileMap->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void ALevelChunk::BeginPlay()
{
	Super::BeginPlay();
	
}

/*void ALevelChunk::OnChunkOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	const AJetpackPlayer* Player = Cast<AJetpackPlayer>(OtherActor);
	if (!Player) return;
	
	AJetpackGameMode* GameMode = Cast<AJetpackGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->SpawnNextChunk();
}*/

// Called every frame
void ALevelChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

