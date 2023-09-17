// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/SpawnManager.h"

#include "TestActor.h"
#include "NetworkActorBase/NetworkActorBase.h"
// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("DataTable'/Game/Data/SpawnClassTable.SpawnClassTable'"));
    if (DataTableAsset.Object != nullptr)
    {
    	SpawnClassTable = DataTableAsset.Object; 
    }
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::SpawnObject(TSubclassOf<ANetworkActorBase> Object, int64 id, FVector3d Pos)
{
	if (SpawnClassTable)
	{
		// 테이블에서 데이터를 검색합니다.
		FName RowName = FName(TEXT("Player"));
		FSubClassData* Row = SpawnClassTable->FindRow<FSubClassData>(RowName, TEXT(""));

		if (Row)
		{
			UWorld* World = GetWorld();
			if (World == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("WorldNull")); // 화면출력
			}
			ANetworkActorBase* networkObject = Cast<ANetworkActorBase>(World->SpawnActor(Row->subclass, &Pos));
        
			if (nullptr != networkObject)			
				networkObject->SetID(id);
			//메모리에 들고 있어야 한다.
		}
	}
}

