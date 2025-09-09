#pragma once
int num = 0;

inline void ActorCache()
{
	ActorTempList.clear();

	Addresses::World = read<UWorld*>(Addresses::base + Offsets::World);
	Cache::OwningGameInstance = Addresses::World->OwningGameInstance();
	Cache::LocalPlayers = Cache::OwningGameInstance->LocalPlayers()->LocalPlayer();
	Addresses::PlayerController = Cache::LocalPlayers->PlayerController();
	Addresses::LocalPawn = Addresses::PlayerController->LocalPawn();
	Cache::PlayerState = Addresses::LocalPawn->PlayerState();
	Cache::TeamIndex = Cache::PlayerState->TeamIndex();
	Cache::CurrentWeapon = Addresses::LocalPawn->CurrentWeapon();
	Cache::RootComponent = Addresses::LocalPawn->RootComponent();
	Cache::LocalRelativeLocation = Cache::RootComponent->RelativeLocation();
	Cache::ClosestDistance = FLT_MAX;
	Cache::ClosestPawn = NULL;

	auto GameState = Addresses::World->GameState();
	auto PlayerArray = GameState->GetPlayerArray();
	for (int i = 0; i < PlayerArray.Size(); i++)
	{
		auto PlayerState = PlayerArray.Get(i);
		AFortPawn* CurrentPawn = PlayerState->GetPawnPrivate();

		auto RelativeLocation = CurrentPawn->RootComponent()->RelativeLocation();

		Actors cached_player{};
		cached_player.PlayerState = PlayerState;
		cached_player.CurrentPawn = CurrentPawn;
		cached_player.RelativeLocation = RelativeLocation;
		cached_player.Distance = Cache::LocalRelativeLocation.distance(RelativeLocation) / 100.0f;
		cached_player.PlayerCount = PlayerArray.Size();

		ActorTempList.push_back(cached_player);
	}

	ActorList.clear();
	ActorList = ActorTempList;

	std::this_thread::sleep_for(std::chrono::milliseconds(150));
}

void WorldCache()
{
	if (Settings::Loot || Settings::LootChests)
	{
		ItemsTempList.clear();

		auto World = read<uintptr_t>(Addresses::base + Offsets::World);
		uintptr_t ItemLevels = read<uintptr_t>(World + Offsets::Levels);
		DWORD LevelCount = read<DWORD>(World + (Offsets::Levels + sizeof(PVOID)));

		for (DWORD levelIndex = 0; levelIndex < LevelCount; ++levelIndex)
		{
			uintptr_t ItemLevel = read<uintptr_t>(ItemLevels + (levelIndex * sizeof(uintptr_t)));
			DWORD ActorCount = read<DWORD>(ItemLevel + (Offsets::AActor + sizeof(PVOID)));

			uintptr_t ItemPawns = read<uintptr_t>(ItemLevel + Offsets::AActor);
			for (DWORD actorIndex = 0; actorIndex < ActorCount; ++actorIndex)
			{
				AFortPawn* ItemPawn = read<AFortPawn*>(ItemPawns + (actorIndex * sizeof(uintptr_t)));
				if (!ItemPawn) continue;

				FVector ItemRelativeLocation = ItemPawn->RootComponent()->RelativeLocation();
				float ItemDistance = Addresses::Location.distance(ItemRelativeLocation) * 0.01f;
				if (ItemDistance > Settings::MaxWorldDistance) continue;

				FName ItemPawnName = read<FName>((uintptr_t)ItemPawn + 0x18);
				auto ItemPawnNameString = ItemPawnName.ToString();

				if (Settings::Loot || Settings::LootChests)
				{
					Items cached_item{};
					cached_item.CurrentPawn = ItemPawn;
					cached_item.Name = ItemPawnNameString;
					cached_item.IsPickup = true;
					cached_item.Distance = ItemDistance;

					ItemsTempList.push_back(cached_item);
				}
			}
		}

		ItemsList = std::move(ItemsTempList);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(150));
}

inline void TestLoop()
{
	for (Actors e : ActorList)
	{
		Addresses::PlayerController->UpdateCamera();

		if (Addresses::LocalPawn && e.Distance > Settings::MaxEspDistance) continue;

		FVector2D Head = Addresses::PlayerController->WorldToScreen(e.CurrentPawn->Mesh()->GetBone(110));
		FVector2D Bottom = Addresses::PlayerController->WorldToScreen(e.CurrentPawn->Mesh()->GetBone(0));

		bool IsVisible = (Settings::VisibleCheck) ? e.CurrentPawn->Mesh()->IsVisible() : true;

		ESPLoop(e.CurrentPawn, Head, Bottom, IsVisible);
		Initialize_RADAR(e.CurrentPawn);

		if (Settings::Aimbot)
		{
			auto WeaponName = Addresses::LocalPawn->CurrentWeapon()->WeaponData()->WeaponName();
			if (strstr(WeaponName.c_str(), _("Pickaxe"))) continue;

			auto dx = Head.X - (HUD_Info::Width / 2);
			auto dy = Head.Y - (HUD_Info::Height / 2);
			auto dist = math::custom_sqrtf(dx * dx + dy * dy);

			if (IsVisible) {
				if (dist < (Settings::FovCircle_Value * HUD_Info::Width / (double)Addresses::FOVAngle) / 2 && dist < Cache::ClosestDistance) {
					Cache::ClosestDistance = dist;
					Cache::ClosestPawn = e.CurrentPawn;
				}
			}
		}
	}
}

void WorldLoop()
{
	if (Settings::Loot || Settings::LootChests)
	{
		for (Items Item : ItemsList)
		{
			if (!Addresses::PlayerController || !Addresses::LocalPawn || !Item.CurrentPawn) continue;

			if (Settings::Loot)
			{
				float DefaultFlyTime = Item.CurrentPawn->DefaultFlyTime();
				if (DefaultFlyTime != 1.5f) continue;
				FVector Pos = Item.CurrentPawn->RootComponent()->RelativeLocation();
				float Distance = Addresses::Location.distance(Pos) * 0.01f;
				if (Distance > Settings::MaxWorldDistance) continue;

				FVector2D Screen = Addresses::PlayerController->WorldToScreen(Pos);
				if (FN::IsInScreen(Screen))
				{
					auto Definition = Item.CurrentPawn->ItemDefinition();
					if (!Definition) continue;

					EFortRarity Tier = Definition->Rarity();
					std::string ItemName = Definition->WeaponName();
					if (ItemName.empty()) continue;

					DrawString(14, ImVec2(Screen.X, Screen.Y), GetColorFromTier(Tier), true, true, ItemName.c_str());
				}
			}
			if (Settings::LootChests)
			{
				EFortPickupSpawnSource SpawnSourceOverride = Item.CurrentPawn->SpawnSourceOverride();
				if (SpawnSourceOverride == EFortPickupSpawnSource::SupplyDrop)
				{
					FVector Pos = Item.CurrentPawn->RootComponent()->RelativeLocation();
					float Distance = Addresses::Location.distance(Pos) * 0.01f;
					if (Distance > Settings::MaxWorldDistance) continue;

					FVector2D Screen = Addresses::PlayerController->WorldToScreen(Pos);
					if (FN::IsInScreen(Screen))
					{
						std::string Text = WideToNarrow(_(L"Chest").decrypt()) + WideToNarrow(_(L" [").decrypt()) + std::to_string(static_cast<int>(Distance)) + WideToNarrow(_(L"m]").decrypt());
						DrawString(14, ImVec2(Screen.X, Screen.Y), ImColor(255, 183, 0, 255), true, true, Text.c_str());
					}
				}
			}
		}
	}
}

inline void ProcessLoop()
{
	AimbotLoop();
	TriggerbotLoop();

	if (Settings::ShowFOVCircle)
	{
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(HUD_Info::Width / 2, HUD_Info::Height / 2), (Settings::FovCircle_Value * HUD_Info::Width / (double)Addresses::FOVAngle) / 2, ImColor(0, 0, 0), 100, 3);
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(HUD_Info::Width / 2, HUD_Info::Height / 2), (Settings::FovCircle_Value * HUD_Info::Width / (double)Addresses::FOVAngle) / 2, ImColor(255, 255, 255, 255), 100, 1.0f);
	}

	if (Settings::ShowCrosshair)
	{
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(HUD_Info::Width / 2 - 7, HUD_Info::Height / 2), ImVec2(HUD_Info::Width / 2 + 1, HUD_Info::Height / 2), ImColor(0, 0, 0, 255), 2.0);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(HUD_Info::Width / 2 + 8, HUD_Info::Height / 2), ImVec2(HUD_Info::Width / 2 + 1, HUD_Info::Height / 2), ImColor(0, 0, 0, 255), 2.0);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(HUD_Info::Width / 2, HUD_Info::Height / 2 - 7), ImVec2(HUD_Info::Width / 2, HUD_Info::Height / 2), ImColor(0, 0, 0, 255), 2.0);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(HUD_Info::Width / 2, HUD_Info::Height / 2 + 8), ImVec2(HUD_Info::Width / 2, HUD_Info::Height / 2), ImColor(0, 0, 0, 255), 2.0);

		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(HUD_Info::Width / 2 - 7, HUD_Info::Height / 2), ImVec2(HUD_Info::Width / 2 + 1, HUD_Info::Height / 2), ImColor(255, 255, 255, 255), 1.0);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(HUD_Info::Width / 2 + 8, HUD_Info::Height / 2), ImVec2(HUD_Info::Width / 2 + 1, HUD_Info::Height / 2), ImColor(255, 255, 255, 255), 1.0);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(HUD_Info::Width / 2, HUD_Info::Height / 2 - 7), ImVec2(HUD_Info::Width / 2, HUD_Info::Height / 2), ImColor(255, 255, 255, 255), 1.0);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(HUD_Info::Width / 2, HUD_Info::Height / 2 + 8), ImVec2(HUD_Info::Width / 2, HUD_Info::Height / 2), ImColor(255, 255, 255, 255), 1.0);
	}

	if (Settings::Loot || Settings::LootChests)
		WorldLoop();
	TestLoop();
}