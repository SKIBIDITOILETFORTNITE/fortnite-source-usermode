#pragma once

class UWorld;

namespace Addresses {
	static UWorld* World = 0;
	static uintptr_t base;
	static FVector Location;
	static FVector Rotation;
	static float FOVAngle;
}

namespace HUD_Info
{
	static int Width;
	static int Height;
}

namespace Settings
{
	static bool Aimbot = 1;
	static bool Prediction = 0;
	static float FovCircle_Value = 15;
	static int Smoothness = 1;
	static int AimBone_Selector = 0;
	static int Aimbone = 110;
	static bool Humanization = 0;
	static int Aimkey = VK_RBUTTON;

	static bool Box = 1;
	static bool CornerBox = 1;
	static bool Skeleton = 1;
	static bool Headdot = 0;
	static bool Nicknames = 1;
	static bool Weapon = 1;
	static bool Platforms = 1;
	static bool Rank = 0;
	static bool Snaplines = 0;
	static bool Distance = 0;
	static bool Wireframes = 0;
	static bool FOVArrows = 0;
	static int MaxEspDistance = 150;

	static bool Loot = 0;
	static bool LootChests = 0;
	static int MaxWorldDistance = 50;

	static bool Triggerbot = 0;
	static bool AlwaysActive = 0;
	static int Triggerkey = VK_CAPITAL;

	static bool Radar = 0;
	static int RadarX = 50;
	static int RadarY = 200;
	static int RadarSize = 200;
	static int RadarDistance = 1000;

	static bool ShowFOVCircle = 1;
	static bool ShowCrosshair = 1;
	static bool Indicators = 1;
	static bool IgnoreDowned = 1;
	static bool VisibleCheck = 1;
	static bool VSYC = 0;

	static bool Head = 1;
	static bool Chest = 0;
	static bool Pelvis = 0;
	static bool ClosestBone = 0;
}

const char* Box_Array[] = { "Cornered", "Bounded" };
static int CurrentBox = 0;