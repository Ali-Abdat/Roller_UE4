// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyLibraryHelper.generated.h"

/**
 * 
 */
UCLASS()
class ROLLER_API UMyLibraryHelper : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(Category = "IO")
		static FString LoadFIleToString(FString FileName);
	UFUNCTION(Category = "IO")
		static TArray<FString> LoadFIleToStringArray(FString FileName);
};
