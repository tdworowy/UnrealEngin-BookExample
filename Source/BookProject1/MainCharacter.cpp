// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerController.h"
// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;

	Health = 85.f;
	MaxHealth = 100.f;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	bAttacking = false;

	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	MainPlayerController = Cast<AMainPlayerController>(GetController());
	
}
void AMainCharacter::MoveForward(float Value)
{	
	if ((Controller != NULL) && (Value != 0.0f))
	{	
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	// FVector Forward = GetActorForwardVector();
	// AddMovementInput(Forward, Value);
}
void AMainCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// UE_LOG(LogTemp, Warning, TEXT("Right: %f"), Value)
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
	// FVector Right = GetActorRightVector();
	// AddMovementInput(Right, Value);
}
// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMainCharacter::LMBDown()
{
//	if (bAttacking) 
	//	return; TODO

	bAttacking = true;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::StopJumping);
	PlayerInputComponent->BindAction("LMBDwon", IE_Pressed, this, &AMainCharacter::LMBDown);

	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMainCharacter::ESCDown);
}
void AMainCharacter::SetHealth(float Amount)
{
	if(Amount > MaxHealth)
	{
		Health = MaxHealth;
	}
	else if (Amount < 0)
	{
		Health = 0;
	}
	else
	{
		Health = Amount;
	}
	
}
void AMainCharacter::ESCDown()
{	
	if (MainPlayerController)
	{	UE_LOG(LogTemp, Warning, TEXT("toggle menu"))
		MainPlayerController->TogglePauseMenu();
	}
}