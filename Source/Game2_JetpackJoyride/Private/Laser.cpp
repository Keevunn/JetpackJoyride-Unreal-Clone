// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser.h"

#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Root);
	CollisionBox->SetBoxExtent(FVector(2.f, 4.f, 4.f));
	
	SpriteRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Sprites"));
	SpriteRoot->SetupAttachment(Root);
	
	TopSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TopSprite"));
	TopSprite->SetupAttachment(SpriteRoot);
	TopSprite->SetRelativeLocation(FVector(0.f, 0.f, (LaserLength/2.f)+1.f));
	
	BottomSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BottomSprite"));
	BottomSprite->SetupAttachment(SpriteRoot);
	BottomSprite->SetRelativeRotation(FRotator(0.f, 0.f, 180.f));
	BottomSprite->SetRelativeLocation(FVector(0.f, 0.f, -((LaserLength/2.f)+1.f)));
	
	MidSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MidSprite"));
	MidSprite->SetupAttachment(SpriteRoot);
	MidSprite->SetRelativeScale3D(FVector(0.5f, 1.f, 1.f));
}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALaser::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	
	UpdateLaserGeometry();
	
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaser::UpdateLaserGeometry() {
	float ScaleZ = LaserLength / OriginalLength;
	FVector MidScale = MidSprite->GetRelativeScale3D();
	MidScale.Z = ScaleZ;
	MidSprite->SetRelativeScale3D(MidScale);
	
	float EndsZ = (LaserLength * 0.5f) + 1.f;
	TopSprite->SetRelativeLocation(FVector(0.f, 0.f, EndsZ));
	BottomSprite->SetRelativeLocation(FVector(0.f, 0.f, -EndsZ));
	
	FVector CollisionExtent = CollisionBox->GetUnscaledBoxExtent();
	CollisionExtent.Z = LaserLength * 0.5f;
	CollisionBox->SetBoxExtent(CollisionExtent);
}

