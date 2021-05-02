// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLibraryHelper.h"
#include "HAL/PlatformFilemanager.h"

FString UMyLibraryHelper::LoadFIleToString(FString FileName)
{
	FString directory = FPaths::ProjectSavedDir();
	FString result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory))
	{
		FString myFile = directory + "/" + FileName;
		FFileHelper::LoadFileToString(result, *myFile);
	}
	return result;
}

TArray<FString> UMyLibraryHelper::LoadFIleToStringArray(FString FileName)
{
	FString directory = FPaths::ProjectSavedDir();
	TArray<FString> results;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory))
	{
		FString myFile = directory + "/" + FileName;
		FFileHelper::LoadFileToStringArray(results, *myFile);
	}
	return results;
}