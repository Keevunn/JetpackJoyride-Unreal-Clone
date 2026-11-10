// Fill out your copyright notice in the Description page of Project Settings.


#include "JetpackPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AJetpackPlayer::AJetpackPlayer() {
	PrimaryActorTick.bCanEverTick = true;
	// TODO should match blueprint values to avoid confusion
	
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	CameraArm->bDoCollisionTest = false;
	CameraArm->SetUsingAbsoluteRotation(true);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	
	Camera->OrthoWidth = 180.f;
	
	GetSprite()->SetRelativeLocation(FVector(0.f, 0.f, 3.f));
	
	GetCapsuleComponent()->InitCapsuleSize(5.4f, 16.f);
	
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->GravityScale = GravityScale;
	Movement->JumpZVelocity = 600.f;
	Movement->AirControl = 0.7f;
	Movement->bConstrainToPlane = true;
	Movement->SetPlaneConstraintNormal(FVector(0.f, 1.f, 0.f)); // Locked to X-Z plane
	Movement->bSnapToPlaneAtStart = true;
	Movement->bUseFlatBaseForFloorChecks = true;
}

void AJetpackPlayer::BeginPlay() {
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
			if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				if (InputMappingContext) 
					InputSubsystem->AddMappingContext(InputMappingContext, 0);
	
}

void AJetpackPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput) return;
	
	/*if (MoveAction) 
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJetpackPlayer::Move);*/
	
	if (JumpAction) {
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AJetpackPlayer::Jump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AJetpackPlayer::StopJumping);
	}
	
	if (FlyAction)
		EnhancedInput->BindAction(FlyAction, ETriggerEvent::Triggered, this, &AJetpackPlayer::Fly);
	
}

void AJetpackPlayer::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	
	UpdateCamera();
	AddMovementInput(FVector::ForwardVector, 0.25f);
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->Velocity.Z = FMath::Clamp(Movement->Velocity.Z, MaxFallSpeed, MaxFlySpeed);
	
	//UpdateAnimation();
}

void AJetpackPlayer::Move(const FInputActionValue& Value) {
	const float MovementValue = Value.Get<float>();
	if (FMath::IsNearlyZero(MovementValue)) return;
	
	AddMovementInput(FVector::ForwardVector, MovementValue);
	UpdateFacing(MovementValue);
}

void AJetpackPlayer::Fly(const FInputActionValue& Value) {
	const float MovementValue = Value.Get<float>();
	if (FMath::IsNearlyZero(MovementValue)) return;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	const float UpAcc = FMath::Abs(Movement->GetGravityZ()) * JetpackAcceleration;
	Movement->AddForce(FVector::UpVector * UpAcc * Movement->Mass);
	
	
}

void AJetpackPlayer::UpdateCamera() {
	const FVector PlayerPostion = GetActorLocation();
	CameraArm->TargetOffset = FVector(CameraLeadX, 0.f, CorridorCentreZ - PlayerPostion.Z);
}

void AJetpackPlayer::UpdateAnimation() {
	if (!GetSprite()) return;
	const UCharacterMovementComponent* Movement = GetCharacterMovement();
	
	if (Movement->IsMovingOnGround()) {
		const FVector Velocity = GetVelocity();
		
		if (!Velocity.IsNearlyZero()) GetSprite()->SetFlipbook(RunFlipbook);
		else GetSprite()->SetFlipbook(IdleFlipbook);
		
		return;
	}
	
	if (GetVelocity().Z >= -100.f) GetSprite()->SetFlipbook(JumpFlipbook);
}

void AJetpackPlayer::UpdateFacing(float MovementValue) {
	if (!Controller) return;
	
	FRotator ControlRot = Controller->GetControlRotation();
	if (MovementValue > 0.f) ControlRot.Yaw = 0.f;
	else ControlRot.Yaw = 180.f;
	
	Controller->SetControlRotation(ControlRot);
}
