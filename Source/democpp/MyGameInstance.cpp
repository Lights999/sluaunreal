// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "GenericPlatformFile.h"
#include "HAL/FileManager.h"

UMyGameInstance::UMyGameInstance() :stateInit("main") {

}

void UMyGameInstance::Init()
{
    stateInit.Init();
}

void UMyGameInstance::Shutdown()
{
    stateInit.Release();
}
