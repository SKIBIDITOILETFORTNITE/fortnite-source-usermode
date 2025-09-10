#pragma once

namespace Offsets {
	constexpr auto World = 0x1827F4E8;			   // World
	constexpr auto LocalPawn = 0x350;			   // AcknowledgedPawn
	constexpr auto Mesh = 0x328;				   // ACharacter
	constexpr auto PlayerController = 0x30;		   // UPlayer
	constexpr auto LocalPlayer = 0x38;			   // UGameInstance
	constexpr auto PlayerState = 0x2C8;			   // APlayerState
	constexpr auto OwningGameInstance = 0x248;	   // UGameInstance
	constexpr auto GameState = 0x1D0;		       // AFortGameStateAthena
	constexpr auto ComponentToWorld = 0x1E0;	   // USceneComponent
	constexpr auto BoneArray = 0x5E8;	           // USceneComponent
	constexpr auto BoneCache = 0x5B8;	           // USceneComponent
	constexpr auto AActor = 0x38;				   // ULevel
	constexpr auto PlayerArray = 0x2c0;			   // ULevel
	constexpr auto RootComponent = 0x1B0;		   // AActor
	constexpr auto RelativeLocation = 0x140;	   // USceneComponent
	constexpr auto PawnPrivate = 0x320;			   // APawn
	constexpr auto TeamIndex = 0x12B9;			   // AFortPlayerStateAthena
	constexpr auto CurrentWeapon = 0x990;		   // AFortWeapon
	constexpr auto bIsReloadingWeapon = 0x3B1;	   // AFortWeapon
	constexpr auto AmmoCount = 0x10D4;	           // AFortWeapon
	constexpr auto DefaultFlyTime = 0x5D4;	       // AFortWeapon
	constexpr auto SpawnSourceOverride = 0xC68;	   // ABuildingContainer
	constexpr auto PickupSpawnSource = 0x2C1;	   // AFortPickup
	constexpr auto WeaponData = 0x570;		       // UFortWeaponItemDefinition
	constexpr auto Rarity = 0xA2;		           // UFortItemDefinition
	constexpr auto bIsDBNO = 0x841;		           // AFortPawn
	constexpr auto Levels = 0x1E8;		           // UWorld
	constexpr auto PrimaryPickupItemEntry = 0x370; // AFortPickup
	constexpr auto Velocity = 0x188;               // USceneComponent
	constexpr auto TargetedFortPawn = 0x18D0;      // AFortPlayerController
	constexpr auto ProjectileSpeed = 0x20e0;
	constexpr auto ProjectileGravity = 0x21c0;
}

namespace VTables
{
	constexpr auto NameStructure = 0xB40;		   // ????
	constexpr auto Platform = 0x430;		       // ????
	constexpr auto HabenaroComponent = 0xa80;	   // ???
	constexpr auto RankProgress = 0xd8;	           // ???
}