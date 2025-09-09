#pragma once

enum class EFortRarity : uint8_t {
	Common = 0,
	Uncommon = 1,
	Rare = 2,
	Epic = 3,
	Legendary = 4,
	Mythic = 5,
	Transcendent = 6,
	Unattainable = 7
};

enum class EFortPickupSpawnSource : uint8_t {
	Unset = 0,
	PlayerElimination = 1,
	Chest = 2,
	SupplyDrop = 3,
	AmmoBox = 4,
	Drone = 5,
	ItemSpawner = 6,
	BotElimination = 7,
	NPCElimination = 8,
	LootDrop = 9,
	TossedByPlayer = 10,
	NPC = 11,
	NPCGift = 12,
	CraftingBench = 13,
	VendingMachine = 14,
	QuestReward = 15,
	EFortPickupSpawnSource_MAX = 16
};