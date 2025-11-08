// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveDataTable.h"

UWaveDataTable::UWaveDataTable() : UDataTable()
{
	RowStruct = FWaveTableRow::StaticStruct();
}
