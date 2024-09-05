// Fill out your copyright notice in the Description page of Project Settings.


#include "MafiaCore/Framework/System/MafiaAssetManager.h"


UMafiaAssetManager::UMafiaAssetManager() 
	: Super()
{

}

UMafiaAssetManager& UMafiaAssetManager::Get()
{
	if (UMafiaAssetManager* Singleton = Cast<UMafiaAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	MAFIA_ULOG(LogMafiaAssetManager, Fatal, TEXT("Can't find UMafiaAssetManager"));

	return *NewObject<UMafiaAssetManager>();
}

void UMafiaAssetManager::Initialize()
{
	Get().LoadPreloadAssets();
}

void UMafiaAssetManager::LoadSyncByPath(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		UObject* LoadedAsset = AssetPath.ResolveObject();
		if (LoadedAsset == nullptr)
		{
			if (UAssetManager::IsInitialized())
			{
				LoadedAsset = UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
			}
			else
			{
				LoadedAsset = AssetPath.TryLoad();
			}
		}

		if (LoadedAsset)
		{
			Get().AddLoadedAsset(AssetPath.GetAssetFName(), LoadedAsset);
		}
		else
		{
			MAFIA_ULOG(LogMafiaAssetManager, Fatal, TEXT("Failed to load asset [%s]"), *AssetPath.ToString());
		}
	}
}

void UMafiaAssetManager::LoadSyncByName(const FName& AssetName)
{
	UMafiaBaseAssetData* AssetData = Get().LoadedAssetData;
	check(AssetData);

	const FSoftObjectPath& AssetPath = AssetData->GetAssetPathByName(AssetName);
	LoadSyncByPath(AssetPath);
}

void UMafiaAssetManager::AddLoadedAsset(const FName& AssetName, const UObject* Asset)
{
	if (AssetName.IsValid() && Asset)
	{
		//FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);

		if (NameToLoadedAsset.Contains(AssetName) == false)
		{
			NameToLoadedAsset.Add(AssetName, Asset);
		}
	}
}

void UMafiaAssetManager::LoadSyncByLabel(const FName& Label)
{
	if (UAssetManager::IsInitialized() == false)
	{
		MAFIA_ULOG(LogMafiaAssetManager, Error, TEXT("AssetManager must be initialized"));
		return;
	}

	UMafiaBaseAssetData* AssetData = Get().LoadedAssetData;
	check(AssetData);

	TArray<FSoftObjectPath> AssetPaths;

	const FAssetSet& AssetSet = AssetData->GetAssetSetByLabel(Label);
	for (const FAssetEntry& AssetEntry : AssetSet.AssetEntries)
	{
		const FSoftObjectPath& AssetPath = AssetEntry.AssetPath;
		LoadSyncByPath(AssetPath);
		if (AssetPath.IsValid())
		{
			AssetPaths.Emplace(AssetPath);
		}
	}

	GetStreamableManager().RequestSyncLoad(AssetPaths);

	for (const FAssetEntry& AssetEntry : AssetSet.AssetEntries)
	{
		const FSoftObjectPath& AssetPath = AssetEntry.AssetPath;
		if (AssetPath.IsValid())
		{
			if (UObject* LoadedAsset = AssetPath.ResolveObject())
			{
				Get().AddLoadedAsset(AssetEntry.AssetName, LoadedAsset);
			}
			else
			{
				MAFIA_ULOG(LogMafiaAssetManager, Fatal, TEXT("Failed to load asset [%s]"), *AssetPath.ToString());
			}
		}
	}
}

void UMafiaAssetManager::LoadAsyncByPath(const FSoftObjectPath& AssetPath, FAsyncLoadCompletedDelegate CompletedDelegate)
{
	if (UAssetManager::IsInitialized() == false)
	{
		MAFIA_ULOG(LogMafiaAssetManager, Error, TEXT("AssetManager must be initialized"));
		return;
	}

	if (AssetPath.IsValid())
	{
		if (UObject* LoadedAsset = AssetPath.ResolveObject())
		{
			Get().AddLoadedAsset(AssetPath.GetAssetFName(), LoadedAsset);
		}
		else
		{
			TArray<FSoftObjectPath> AssetPaths;
			AssetPaths.Add(AssetPath);

			TSharedPtr<FStreamableHandle> Handle = GetStreamableManager().RequestAsyncLoad(AssetPaths);

			Handle->BindCompleteDelegate(FStreamableDelegate::CreateLambda([AssetName = AssetPath.GetAssetFName(), AssetPath, CompleteDelegate = MoveTemp(CompletedDelegate)]()
				{
					UObject* LoadedAsset = AssetPath.ResolveObject();
					Get().AddLoadedAsset(AssetName, LoadedAsset);
					if (CompleteDelegate.IsBound())
						CompleteDelegate.Execute(AssetName, LoadedAsset);
				}));
		}
	}
}

void UMafiaAssetManager::LoadAsyncByName(const FName& AssetName, FAsyncLoadCompletedDelegate CompletedDelegate)
{
	if (UAssetManager::IsInitialized() == false)
	{
		MAFIA_ULOG(LogMafiaAssetManager, Error, TEXT("AssetManager must be initialized"));
		return;
	}

	UMafiaBaseAssetData* AssetData = Get().LoadedAssetData;
	check(AssetData);

	const FSoftObjectPath& AssetPath = AssetData->GetAssetPathByName(AssetName);
	LoadAsyncByPath(AssetPath, CompletedDelegate);
}

void UMafiaAssetManager::ReleaseByPath(const FSoftObjectPath& AssetPath)
{
	FName AssetName = AssetPath.GetAssetFName();
	ReleaseByName(AssetName);
}

void UMafiaAssetManager::ReleaseByName(const FName& AssetName)
{
	UMafiaAssetManager& AssetManager = Get();
	if (AssetManager.NameToLoadedAsset.Contains(AssetName))
	{
		AssetManager.NameToLoadedAsset.Remove(AssetName);
	}
	else
	{
		MAFIA_ULOG(LogMafiaAssetManager, Log, TEXT("Can't find loaded asset by assetName [%s]."), *AssetName.ToString());
	}
}

void UMafiaAssetManager::ReleaseByLabel(const FName& Label)
{
	UMafiaAssetManager& AssetManager = Get();
	UMafiaBaseAssetData* LoadedAssetData = AssetManager.LoadedAssetData;
	const FAssetSet& AssetSet = LoadedAssetData->GetAssetSetByLabel(Label);

	for (const FAssetEntry& AssetEntry : AssetSet.AssetEntries)
	{
		const FName& AssetName = AssetEntry.AssetName;
		if (AssetManager.NameToLoadedAsset.Contains(AssetName))
		{
			AssetManager.NameToLoadedAsset.Remove(AssetName);
		}
		else
		{
			MAFIA_ULOG(LogMafiaAssetManager, Log, TEXT("Can't find loaded asset by assetName [%s]."), *AssetName.ToString());
		}
	}
}

void UMafiaAssetManager::ReleaseAll()
{
	UMafiaAssetManager& AssetManager = Get();
	AssetManager.NameToLoadedAsset.Reset();
}

void UMafiaAssetManager::LoadPreloadAssets()
{
	if (LoadedAssetData)
		return;

	UMafiaBaseAssetData* AssetData = nullptr;
	FPrimaryAssetType PrimaryAssetType(UMafiaBaseAssetData::StaticClass()->GetFName());
	TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAssetsWithType(PrimaryAssetType);
	if (Handle.IsValid())
	{
		Handle->WaitUntilComplete(0.f, false);
		AssetData = Cast<UMafiaBaseAssetData>(Handle->GetLoadedAsset());
	}

	if (AssetData)
	{
		LoadedAssetData = AssetData;
		LoadSyncByLabel("Preload");
	}
	else
	{
		MAFIA_ULOG(LogMafiaAssetManager, Fatal, TEXT("Failed to load AssetData asset type [%s]."), *PrimaryAssetType.ToString());
	}
}