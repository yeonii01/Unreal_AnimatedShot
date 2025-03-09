// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ASShop.h"
#include "Physics/ASCollision.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AASShop::AASShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    // Static Mesh 생성 및 Root 설정
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Trigger Box 생성 및 초기화
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);

    TriggerBox->SetCollisionProfileName(CPROFILE_ASTRIGGER);
    TriggerBox->SetBoxExtent(FVector(60.f, 42.f, 30.f));
    TriggerBox->SetRelativeLocation(FVector(0.f, 100.f, 60.f));
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AASShop::OnOverlapBegin);

    ShopWidgetInstance = nullptr;
}

// Called when the game starts or when spawned
void AASShop::BeginPlay()
{
	Super::BeginPlay();

    if (ShopWidgetClass)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            ShopWidgetInstance = CreateWidget<UUserWidget>(PlayerController, ShopWidgetClass);
            if (ShopWidgetInstance)
            {
                ShopWidgetInstance->RemoveFromParent();
            }
        }
    }
}

// Called every frame
void AASShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AASShop::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
    if (ShopWidgetInstance && !ShopWidgetInstance->IsInViewport()) // 한 번만 UI 추가
    {
        ShopWidgetInstance->AddToViewport();

        // 마우스 커서 활성화
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = true;
            PlayerController->SetInputMode(FInputModeUIOnly());

            PlayerController->SetIgnoreMoveInput(true);
            PlayerController->SetIgnoreLookInput(true);
        }
    }
}
