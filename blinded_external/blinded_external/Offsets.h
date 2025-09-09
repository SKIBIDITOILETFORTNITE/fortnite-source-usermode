#pragma once

namespace Offsets {
	constexpr auto World = 0x169D6108;			   // World
	constexpr auto LocalPawn = 0x350;			   // AcknowledgedPawn
	constexpr auto Mesh = 0x328;				   // ACharacter
	constexpr auto PlayerController = 0x30;		   // UPlayer
	constexpr auto LocalPlayer = 0x38;			   // UGameInstance
	constexpr auto PlayerState = 0x2C8;			   // APlayerState
	constexpr auto OwningGameInstance = 0x1f0;	   // UGameInstance
	constexpr auto GameState = 0x178;		       // AFortGameStateAthena
	constexpr auto ComponentToWorld = 0x1E0;	   // USceneComponent
	constexpr auto BoneArray = 0x5A8;	           // USceneComponent
	constexpr auto BoneCache = 0x5B8;	           // USceneComponent
	constexpr auto AActor = 0x1A8;				   // ULevel
	constexpr auto PlayerArray = 0x2c0;			   // ULevel
	constexpr auto RootComponent = 0x1B0;		   // AActor
	constexpr auto RelativeLocation = 0x138;	   // USceneComponent
	constexpr auto PawnPrivate = 0x320;			   // APawn
	constexpr auto TeamIndex = 0x1261;			   // AFortPlayerStateAthena
	constexpr auto CurrentWeapon = 0xa80;		   // AFortWeapon
	constexpr auto bIsReloadingWeapon = 0x3c1;	   // AFortWeapon
	constexpr auto AmmoCount = 0x10bc;	           // AFortWeapon
	constexpr auto DefaultFlyTime = 0x624;	       // AFortWeapon
	constexpr auto SpawnSourceOverride = 0xca0;	   // ABuildingContainer
	constexpr auto PickupSpawnSource = 0x2c1;	   // AFortPickup
	constexpr auto WeaponData = 0x690;		       // UFortWeaponItemDefinition
	constexpr auto Rarity = 0xa2;		           // UFortItemDefinition
	constexpr auto bIsDBNO = 0x962;		           // AFortPawn
	constexpr auto Levels = 0x190;		           // UWorld
	constexpr auto PrimaryPickupItemEntry = 0x370; // AFortPickup
	constexpr auto Velocity = 0x180;               // USceneComponent
	constexpr auto TargetedFortPawn = 0x1930;      // AFortPlayerController
	constexpr auto ProjectileSpeed = 0x1D20;
	constexpr auto ProjectileGravity = 0x1D88;
}

namespace VTables
{
	constexpr auto NameStructure = 0xaf8;		   // ????
	constexpr auto Platform = 0x430;		       // ????
	constexpr auto HabenaroComponent = 0xa38;	   // ???
	constexpr auto RankProgress = 0xd0;	           // ???
}