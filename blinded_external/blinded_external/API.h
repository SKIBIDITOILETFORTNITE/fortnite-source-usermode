#pragma once

bool is_bad_ptr(uintptr_t* ptr) {
	if (!ptr || (uint64_t)ptr > 0xffffffffffff)
		return true;

	__try
	{
		volatile auto result = *ptr;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return true;
	}
	return false;
}

namespace FN {
	static bool IsValidPointer(const uint64_t adress)
	{
		if (adress <= 0x400000 || adress == 0xCCCCCCCCCCCCCCCC || reinterpret_cast<void*>(adress) == nullptr || adress >
			0x7FFFFFFFFFFFFFFF) {
			return false;
		}
		return true;
	}

	auto IsInScreen(FVector2D Screen) -> bool {
		if (Screen.X > 0 && Screen.X < HUD_Info::Width && Screen.Y > 0 && Screen.Y < HUD_Info::Height) return true;
		else return false;
	}
}

class USkeletalMeshComponent {
public:

	FVector GetBone(int id) {
		int32_t ActiveTransforms = (memory.Read<int32_t>((uintptr_t)this + (Offsets::BoneArray + 0x48)) * std::size_t(0x10));
		auto bone_array = memory.Read<TArray1<FTransform>>((uintptr_t)this + Offsets::BoneArray + ActiveTransforms);
		auto BoneTransform = memory.Read<FTransform>(bone_array.Array + (id * 0x60));
		FTransform ComponentToWorld = memory.Read<FTransform>((uintptr_t)this + Offsets::ComponentToWorld);
		D3DMATRIX Matrix = MatrixMultiplication(
			BoneTransform.ToMatrixWithScale().ToD3DMATRIX(),
			ComponentToWorld.ToMatrixWithScale().ToD3DMATRIX()
		);
		return FVector(Matrix._41, Matrix._42, Matrix._43);
	}

	bool IsVisible() {
		auto World = memory.Read<uintptr_t>(driver::base_address + Offsets::World);
		double Seconds = memory.Read<double>(World + 0x148);
		float LastRenderTime = memory.Read<float>(uintptr_t(this) + 0x30C);
		return Seconds - LastRenderTime <= 0.6;
	}

	BOOL bUseAsOccluder(BOOL Value)
	{
		memory.Write<BOOL>(uintptr_t(this) + 0x26d, Value);
		return TRUE;
	}
};

class APlayerState {
public:

	char TeamIndex() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<char>(uintptr_t(this) + Offsets::TeamIndex);
	}

	std::string GetPlayerName()
	{
		__int64 FString = memory.Read<__int64>(uintptr_t(this) + VTables::NameStructure);
		int Length = memory.Read<int>(FString + 16);
		uintptr_t FText = memory.Read<__int64>(FString + 8);
		__int64 v6 = Length;
		if (!v6) return std::string("BOT");
		wchar_t* NameBuffer = new wchar_t[Length];
		driver::ReadPhysical((PVOID)FText, NameBuffer, Length * sizeof(wchar_t));
		char v21;
		int v22;
		int i;
		int v25;
		WORD* v23;
		v21 = v6 - 1;
		if (!(DWORD)v6)
			v21 = 0;
		v22 = 0;
		v23 = (WORD*)NameBuffer;
		for (i = (v21) & 3; ; *v23++ += i & 7)
		{
			v25 = v6 - 1;
			if (!(DWORD)v6)
				v25 = 0;
			if (v22 >= v25)
				break;
			i += 3;
			++v22;
		}
		std::wstring wbuffer{ NameBuffer };
		return std::string(wbuffer.begin(), wbuffer.end());
	}

	std::string GetPlatform()
	{
		uintptr_t platform_ptr = memory.Read<uintptr_t>(uintptr_t(this) + VTables::Platform);
		wchar_t platform_char[64] = { 0 };
		driver::ReadPhysical(reinterpret_cast<PVOID>(platform_ptr), reinterpret_cast<uint8_t*>(platform_char), sizeof(platform_char));
		std::wstring platform_wide_str(platform_char);
		return std::string(platform_wide_str.begin(), platform_wide_str.end());
	}

	int32_t GetRank()
	{
		uintptr_t Habenaro = memory.Read<uintptr_t>(uintptr_t(this) + VTables::HabenaroComponent);
		return memory.Read<int32_t>(Habenaro + VTables::RankProgress + 0x10);
	}
};

class ARootComponent {
public:

	FVector RelativeLocation() {
		if (!FN::IsValidPointer((uintptr_t)this)) return { 0, 0, 0 };
		return memory.Read<FVector>(uintptr_t(this) + Offsets::RelativeLocation);
	}

	FVector Velocity() {
		return memory.Read<FVector>((uintptr_t)this + Offsets::Velocity);
	}
};

class AFortItemDefinition {
public:

	enum class EFortRarity Rarity()
	{
		return memory.Read<EFortRarity>(uintptr_t(this) + Offsets::Rarity);
	}

	std::string WeaponName()
	{
		std::string WeaponName;
		uint64_t ItemName = memory.Read<uint64_t>(uintptr_t(this) + 0x40);
		uint64_t FData = memory.Read<uint64_t>(ItemName + 0x18);
		int FLength = memory.Read<int>(ItemName + 0x20);
		if (FLength > 0 && FLength < 50)
		{
			wchar_t* WeaponBuffer = new wchar_t[FLength];
			driver::ReadPhysical((void*)FData, (PVOID)WeaponBuffer, FLength * sizeof(wchar_t));
			std::wstring wstr_buf(WeaponBuffer);
			WeaponName.append(std::string(wstr_buf.begin(), wstr_buf.end()));
			delete[] WeaponBuffer;
		}
		return WeaponName;
	}
};

class AFortWeapon {
public:

	AFortItemDefinition* WeaponData() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<AFortItemDefinition*>(uintptr_t(this) + Offsets::WeaponData);
	}

	BOOL IsReloadingWeapon() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<BOOL>(uintptr_t(this) + Offsets::bIsReloadingWeapon);
	}

	FLOAT GetProjectileSpeed()
	{
		return memory.Read<FLOAT>((uintptr_t)this + Offsets::ProjectileSpeed);
	}
	FLOAT GetProjectileGravity()
	{
		return memory.Read<FLOAT>((uintptr_t)this + Offsets::ProjectileGravity);
	}

	int AmmoCount() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<int>(uintptr_t(this) + Offsets::AmmoCount);
	}
};

class AFortPawn {
public:

	USkeletalMeshComponent* Mesh() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<USkeletalMeshComponent*>(uintptr_t(this) + Offsets::Mesh);
	}

	AFortWeapon* CurrentWeapon() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<AFortWeapon*>(uintptr_t(this) + Offsets::CurrentWeapon);
	}

	ARootComponent* RootComponent() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<ARootComponent*>(uintptr_t(this) + Offsets::RootComponent);
	}

	APlayerState* PlayerState() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<APlayerState*>(uintptr_t(this) + Offsets::PlayerState);
	}

	AFortItemDefinition* ItemDefinition()
	{
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<AFortItemDefinition*>(uintptr_t(this) + Offsets::PrimaryPickupItemEntry + 0x18);
	}

	FBounds ActorBounds(FVector2D head2d, FVector2D bottom2d, float size)
	{
		FBounds a_bounds;
		a_bounds.Top = head2d.Y;
		a_bounds.Bottom = bottom2d.Y;
		a_bounds.Left = (head2d.X - (size / 2));
		a_bounds.Right = bottom2d.X + (size / 2);
		return a_bounds;
	}

	BOOL IsDBNO()
	{
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return (memory.Read <BYTE>(uintptr_t(this) + Offsets::bIsDBNO) >> 4) & 1;
	}

	FLOAT DefaultFlyTime()
	{
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<FLOAT>(uintptr_t(this) + Offsets::DefaultFlyTime);
	}

	EFortPickupSpawnSource SpawnSourceOverride()
	{
		return memory.Read<EFortPickupSpawnSource>(uintptr_t(this) + Offsets::SpawnSourceOverride);
	}

	EFortPickupSpawnSource PickupSpawnSource()
	{
		return memory.Read<EFortPickupSpawnSource>(uintptr_t(this) + Offsets::PickupSpawnSource);
	}
};

class AFortPlayerState : public APlayerState
{
public:
	auto GetPawnPrivate() -> AFortPawn*
	{
		return memory.Read<AFortPawn*>((uintptr_t)this + Offsets::PawnPrivate);
	}
};

class APlayerController {
public:

	AFortPawn* LocalPawn() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<AFortPawn*>(uintptr_t(this) + Offsets::LocalPawn);
	}

	AFortPawn* GetTargetedPawn() {
		return memory.Read<AFortPawn*>((uintptr_t)this + Offsets::TargetedFortPawn);
	}

	void UpdateCamera() {
		auto location_pointer = memory.Read<uintptr_t>((uintptr_t)Addresses::World + 0x178);
		auto rotation_pointer = memory.Read<uintptr_t>((uintptr_t)Addresses::World + 0x188);

		struct FNRot
		{
			double a;
			char pad_0008[24];
			double b;
			char pad_0028[424];
			double c;
		} fnRot;

		fnRot.a = memory.Read<double>(rotation_pointer);
		fnRot.b = memory.Read<double>(rotation_pointer + 0x20);
		fnRot.c = memory.Read<double>(rotation_pointer + 0x1D0);

		Addresses::Location = memory.Read<FVector>(location_pointer);
		Addresses::Rotation.X = asin(fnRot.c) * (180.0 / M_PI);
		Addresses::Rotation.Y = ((atan2(fnRot.a * -1, fnRot.b) * (180.0 / M_PI)) * -1) * -1;
		Addresses::FOVAngle = memory.Read<float>((uintptr_t)this + 0x3AC) * 90.f;
	}


	inline FVector2D WorldToScreen(FVector WorldLocation)
	{
		if (WorldLocation.X == 0)
			return FVector2D(0, 0);

		D3DMATRIX tempMatrix = ToMatrix(Addresses::Rotation);

		FVector vAxisX = FVector{ tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2] };
		FVector vAxisY = FVector{ tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2] };
		FVector vAxisZ = FVector{ tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2] };

		FVector vDelta = WorldLocation - Addresses::Location;
		FVector vTransformed = FVector{ vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX) };

		if (vTransformed.Z < 1.f)
			vTransformed.Z = 1.f;

		return FVector2D((HUD_Info::Width / 2.0f) + vTransformed.X * (((HUD_Info::Width / 2.0f) / tanf(Addresses::FOVAngle * (float)M_PI / 360.f))) / vTransformed.Z, (HUD_Info::Height / 2.0f) - vTransformed.Y * (((HUD_Info::Width / 2.0f) / tanf(Addresses::FOVAngle * (float)M_PI / 360.f))) / vTransformed.Z);
	}
};

class ULocalPlayer {
public:
	APlayerController* PlayerController() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<APlayerController*>(uintptr_t(this) + Offsets::PlayerController);
	}
};

class ULocalPlayers {
public:
	ULocalPlayer* LocalPlayer() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<ULocalPlayer*>(uintptr_t(this));
	}
};

class UGameInstance {
public:
	ULocalPlayers* LocalPlayers() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<ULocalPlayers*>(uintptr_t(this) + Offsets::LocalPlayer);
	}
};

class AActors {
public:

	AFortPawn* GetCurrentActor(int i) {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return (AFortPawn*)(*(uintptr_t*)&this[i * sizeof(uintptr_t)]);
	}
};

class AGameStateBase {
public:

	TArray<AFortPlayerState*> GetPlayerArray()
	{
		return memory.Read<TArray<AFortPlayerState*>>((uintptr_t)this + Offsets::PlayerArray);
	}

};

class UWorld {
public:

	UGameInstance* OwningGameInstance() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<UGameInstance*>(uintptr_t(this) + Offsets::OwningGameInstance);
	}

	AGameStateBase* GameState() {
		if (!FN::IsValidPointer((uintptr_t)this)) return 0;
		return memory.Read<AGameStateBase*>(uintptr_t(this) + Offsets::GameState);
	}
};

typedef struct Actors
{
	AFortPlayerState* PlayerState;
	AFortPawn* CurrentPawn;
	USkeletalMeshComponent* Mesh;
	FVector RelativeLocation;
	float Distance;
	std::string Weapon;
	int PlayerCount;
} Actors;
std::vector<Actors> ActorList;
std::vector<Actors> ActorTempList;

typedef struct Items
{
	AFortPawn* CurrentPawn;
	std::string Name;
	bool IsPickup;
	float Distance;
} Items;
std::vector<Items> inline ItemsList;
std::vector<Items> ItemsTempList;


namespace Addresses {
	static APlayerController* PlayerController;
	static AFortPawn* LocalPawn;
}

namespace Cache
{
	static UGameInstance* OwningGameInstance;
	static ULocalPlayer* LocalPlayers;
	static APlayerState* PlayerState;
	static AFortWeapon* CurrentWeapon;
	static ARootComponent* RootComponent;
	static FVector LocalRelativeLocation;
	static char TeamIndex;
	static double ClosestDistance;
	static AFortPawn* ClosestPawn;
}