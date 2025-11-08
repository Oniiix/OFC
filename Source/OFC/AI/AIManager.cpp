// Fill out your copyright notice in the Description page of Project Settings.

#include "AIManager.h"
#include "Duck.h"
#include "Swiper.h"
#include "WaveDataTable.h"
#include "Kismet/KismetSystemLibrary.h"

void UAIManager::Init(AGameModeBase* _gameMode)
{
	gameMode = _gameMode;
	if (!gameMode)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Failed to get gameMode in AIManager", true, true, FLinearColor::Red);
		return;
	}
	if (!waves)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Waves is not set in AIManager", true, true, FLinearColor::Red);
		return;
	}
	if (!duckRef)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "DuckRef is not set in AIManager", true, true, FLinearColor::Red);
		return;
	}
	if (!swiperRef)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "SwiperRef is not set in AIManager", true, true, FLinearColor::Red);
		return;
	}

	/// Check NavigationSystem availability
	navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!navSystem)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Failed to find navigation system on AIManager", true, true, FLinearColor::Red);
		return;
	}

	currentWave = -1;
	waveSwiperMax = 0;
	swipersDead = 0;
	swiperCount = 0;
	duckCount = 0;

}

void UAIManager::SetEnclosure(AEnclosureActor* _enclosure)
{
	enclosure = _enclosure;
	enclosure->GetSpawnPoints(this);
}

void UAIManager::GetSpawnPoint(TArray<AActor*> _listSpawnPoint)
{
	listSpawnPoint = _listSpawnPoint;
	LaunchNextWave();
}

void UAIManager::TestWave()
{
	// All swipers dead => end wave
	//if (swipersDead == waveSwiperMax)
	//{
	//	const FName _rowName = FName(FString::FromInt(currentWave));
	//	FWaveTableRow* _waveRow = waves->FindRow<FWaveTableRow>(_rowName, TABLE_CONTEXT, true);
	//	if (!_waveRow)
	//	{
	//		// Fin du game
	//		onGameOver.Broadcast();
	//		return;
	//	}
	//	else
	//	{
	//		onEndWave.Broadcast();
	//	}
	//}
}

void UAIManager::RemoveDuck(ADuck* _duck)
{
	listDuck.Remove(_duck);
	if (listDuck.IsEmpty())
		onGameOver.Broadcast();
}

void UAIManager::AddFreeDuck(ADuck* _duck)
{
	listFreeDuck.Add(_duck);
}

void UAIManager::RemoveFreeDuck(ADuck* _duck)
{
	listFreeDuck.Remove(_duck);
}

void UAIManager::SwiperDead(ASwiper* _swiper)
{
	listSwiper.Remove(_swiper);
	swipersDead++;
	UKismetSystemLibrary::PrintString(this, FString::FromInt(swipersDead) + " | " + FString::FromInt(waveSwiperMax) + " | " + FString::FromInt(swiperCount), true, true, FColor::Blue);
	if (swipersDead == waveSwiperMax)
	{
		if (currentWave < waves->GetRowMap().Num() - 1)
		{
			const int _duckCount = listDuck.Num();
			for (int i = 0; i < _duckCount; i++)
			{
				if (listDuck[i])
					listDuck[i]->SetLifeSpan(0.1);
			}
			listDuck.Empty();
			listFreeDuck.Empty();
			onEndWave.Broadcast();
			TArray<UObject*> _objects = onEndWave.GetAllObjects();
			for (UObject* _object : _objects)
			{
				UKismetSystemLibrary::PrintString(this, _object->GetName(), true, true, FColor::FColor::Purple);
			}
			return;
		}
		onFinishAllWawes.Broadcast();
	}
}



void UAIManager::LaunchNextWave()
{
	currentWave++;
	swipersDead = 0;
	swiperCount = 0;
	// Get wave data
	const FName _rowName = FName(FString::FromInt(currentWave));
	FWaveTableRow* _waveRow = waves->FindRow<FWaveTableRow>(_rowName, TABLE_CONTEXT, true);

	waveSwiperMax = _waveRow->swipersMax;
	// Spawn ducks
	const int _duckToSpawnCount = _waveRow->duckToSpawn;
	UKismetSystemLibrary::PrintString(this, FString::FromInt(_duckToSpawnCount), true, true, FColor::Yellow);
	for (int i = 0; i < _duckToSpawnCount; i++)
	{
		/// Get available location
		FVector _res = enclosure->GetActorLocation();
		FVector _origin;
		FVector _extent;
		enclosure->GetActorBounds(false, _origin, _extent, true);
		FNavLocation _point;

		for (int j = 0; j < 100; j++)
		{
			const bool _success = navSystem->GetRandomPointInNavigableRadius(_res, spawnDuckRadius, _point);
			if (_success && FVector::Dist(_res, _point.Location) >= _extent.Length()) // not too close of enclosure
			{
				FActorSpawnParameters _param;
				_param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				ADuck* _newDuck = GetWorld()->SpawnActor<ADuck>(duckRef, _point.Location, FRotator::ZeroRotator, _param);
				_newDuck->OnDead().AddDynamic(this, &UAIManager::RemoveDuck);
				_newDuck->OnFree().AddDynamic(this, &UAIManager::AddFreeDuck);
				_newDuck->OnNotFree().AddDynamic(this, &UAIManager::RemoveFreeDuck);
				listDuck.Add(_newDuck);
				listFreeDuck.Add(_newDuck);
				break;
			}
		}
	}
	UKismetSystemLibrary::PrintString(this, FString::FromInt(listFreeDuck.Num()), true, true, FColor::Yellow);

	// Spawn Swiper	
	FTimerDelegate _DelegateSpawnSwipers;
	_DelegateSpawnSwipers.BindUFunction(this, FName("SpawnSwipers"), _waveRow->swiperToSpawn);
	GetWorld()->GetTimerManager().SetTimer(timerSpawnSwipers, _DelegateSpawnSwipers, _waveRow->swiperFrequency, true);
}

void UAIManager::SpawnSwipers(const int _spawnCount)
{
	int _count;
	if (swiperCount + _spawnCount >= waveSwiperMax)
	{
		_count = waveSwiperMax - swiperCount;
		GetWorld()->GetTimerManager().ClearTimer(timerSpawnSwipers);
	}
	else
		_count = _spawnCount;

	for (int i = 0; i < _count; i++)
	{
		FVector _spawnLocation;
		const int _spawnPointCount = listSpawnPoint.Num();
		if (_spawnPointCount == 0)
			_spawnLocation = FVector(1000, 1000, 10);
		else
		{
			const int _i = FMath::RandRange(0, _spawnPointCount - 1);
			_spawnLocation = listSpawnPoint[_i]->GetActorLocation();
		}
		FActorSpawnParameters _param;
		_param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ASwiper* _newSwiper = GetWorld()->SpawnActor<ASwiper>(swiperRef, _spawnLocation, FRotator::ZeroRotator, _param);
		_newSwiper->OnDead().AddDynamic(this, &UAIManager::SwiperDead);

		_newSwiper->OnHunting().AddDynamic(this, &UAIManager::GiveTargetToSwiper);

		GiveTargetToSwiper(_newSwiper);

		listSwiper.Add(_newSwiper);
		swiperCount++;
	}
}

void UAIManager::GiveTargetToSwiper(ASwiper* _newSwiper)
{
	UKismetSystemLibrary::PrintString(this, _newSwiper->IsBubbled() ? "il est Bubble" : "Il est pas Bubble", true, false, _newSwiper->IsBubbled() ? FColor::Green : FColor::Red, 5);

	const int _freeDuckCount = listFreeDuck.Num();

	if (_freeDuckCount == 0) // no more free duck => focus enclosure
		_newSwiper->SetTarget(enclosure);
	else
	{
		const int _index = FMath::RandRange(0, _freeDuckCount - 1);
		ADuck* _duck = listFreeDuck[_index];
		RemoveFreeDuck(_duck);
		_newSwiper->SetTarget(_duck);
		_duck->SetSwiper(_newSwiper);
	}
}