#pragma once

static BOOL WritePrivateProfileInt(LPCSTR lpAppName, LPCSTR lpKeyName, int nInteger, LPCSTR lpFileName) {
    char lpString[1024];
    sprintf(lpString, _("%d"), nInteger);
    return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString, lpFileName);
}
static BOOL WritePrivateProfileFloat(LPCSTR lpAppName, LPCSTR lpKeyName, float nInteger, LPCSTR lpFileName) {
    char lpString[1024];
    sprintf(lpString, _("%f"), nInteger);
    return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString, lpFileName);
}
static float GetPrivateProfileFloat(LPCSTR lpAppName, LPCSTR lpKeyName, FLOAT flDefault, LPCSTR lpFileName)
{
    char szData[32];

    GetPrivateProfileStringA(lpAppName, lpKeyName, std::to_string(flDefault).c_str(), szData, 32, lpFileName);

    return (float)atof(szData);
}
std::string GetPCUserName() {
    char username[256];
    DWORD size = sizeof(username);
    if (GetUserNameA(username, &size)) {
        return std::string(username);
    }
    return _("User").decrypt();
}

static void SaveSettings(LPCSTR path)
{
    WritePrivateProfileInt((_("Visuals")), (_("m_playeresp")), Settings::Box, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_boxstyle")), CurrentBox, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_skeleton")), Settings::Skeleton, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_playername")), Settings::Nicknames, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_playerweapons")), Settings::Weapon, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_playerplatforms")), Settings::Platforms, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_playerrank")), Settings::Rank, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_snaplines")), Settings::Snaplines, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_playerdistance")), Settings::Distance, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_wireframe")), Settings::Wireframes, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_radar")), Settings::Radar, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_radarsize")), Settings::RadarSize, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_warningarrows")), Settings::FOVArrows, path);

    WritePrivateProfileInt((_("Visuals")), (_("m_loot")), Settings::Loot, path);
    WritePrivateProfileInt((_("Visuals")), (_("m_chests")), Settings::LootChests, path);

    WritePrivateProfileInt((_("Aimbot")), (_("m_aimbot")), Settings::Aimbot, path);
    WritePrivateProfileInt((_("Aimbot")), (_("m_smooth")), Settings::Smoothness, path);
    WritePrivateProfileFloat((_("Aimbot")), (_("m_aimbotfov")), Settings::FovCircle_Value, path);
    WritePrivateProfileFloat((_("Aimbot")), (_("m_humanization")), Settings::Humanization, path);
    WritePrivateProfileFloat((_("Aimbot")), (_("m_ignoredowned")), Settings::IgnoreDowned, path);
    WritePrivateProfileFloat((_("Aimbot")), (_("m_prediction")), Settings::Prediction, path);
    WritePrivateProfileInt((_("Aimbot")), (_("m_circleline")), Settings::Headdot, path);
    WritePrivateProfileInt((_("Aimbot")), (_("m_aimkey")), Settings::Aimkey, path);
    WritePrivateProfileInt((_("Aimbot")), (_("m_triggerkey")), Settings::Triggerkey, path);
    WritePrivateProfileInt((_("Aimbot")), (_("m_head")), Settings::Head, path);
    WritePrivateProfileInt((_("Aimbot")), (_("m_chest")), Settings::Chest, path);
    WritePrivateProfileInt((_("Aimbot")), (_("m_pelvis")), Settings::Pelvis, path);
    WritePrivateProfileInt((_("Aimbot")), (_("m_triggerbot")), Settings::Triggerbot, path);
    WritePrivateProfileInt((_("Aimbot")), (_("m_alwaysactive")), Settings::AlwaysActive, path);

    WritePrivateProfileInt((_("Misc")), (_("m_showfov")), Settings::ShowFOVCircle, path);
    WritePrivateProfileInt((_("Misc")), (_("m_showcrosshair")), Settings::ShowCrosshair, path);
    WritePrivateProfileFloat((_("Misc")), (_("m_indicators")), Settings::Indicators, path);
    WritePrivateProfileFloat((_("Misc")), (_("m_visiblecheck")), Settings::VisibleCheck, path);
    WritePrivateProfileFloat((_("Misc")), (_("m_vsyc")), Settings::VSYC, path);

    WritePrivateProfileInt((_("Distance")), (_("m_playerdistance")), Settings::MaxEspDistance, path);
    WritePrivateProfileInt((_("Distance")), (_("m_lootdistance")), Settings::MaxWorldDistance, path);
}

static void LoadSettings(LPCSTR path)
{
    Settings::Box = GetPrivateProfileIntA((_("Visuals")), (_("m_playeresp")), Settings::Box, path);
    Settings::Radar = GetPrivateProfileIntA((_("Visuals")), (_("m_radar")), Settings::Radar, path);
    Settings::RadarSize = GetPrivateProfileIntA((_("Visuals")), (_("m_radarsize")), Settings::RadarSize, path);
    CurrentBox = GetPrivateProfileIntA((_("Visuals")), (_("m_boxstyle")), CurrentBox, path);
    Settings::Skeleton = GetPrivateProfileIntA((_("Visuals")), (_("m_skeleton")), Settings::Skeleton, path);
    Settings::Nicknames = GetPrivateProfileIntA((_("Visuals")), (_("m_playername")), Settings::Nicknames, path);
    Settings::Weapon = GetPrivateProfileIntA((_("Visuals")), (_("m_playerweapons")), Settings::Weapon, path);
    Settings::Platforms = GetPrivateProfileIntA((_("Visuals")), (_("m_playerplatforms")), Settings::Platforms, path);
    Settings::Rank = GetPrivateProfileIntA((_("Visuals")), (_("m_playerrank")), Settings::Rank, path);
    Settings::Snaplines = GetPrivateProfileIntA((_("Visuals")), (_("m_snaplines")), Settings::Snaplines, path);
    Settings::Distance = GetPrivateProfileIntA((_("Visuals")), (_("m_playerdistance")), Settings::Distance, path);
    Settings::Wireframes = GetPrivateProfileIntA((_("Visuals")), (_("m_wireframe")), Settings::Wireframes, path);
    Settings::FOVArrows = GetPrivateProfileIntA((_("Visuals")), (_("m_warningarrows")), Settings::FOVArrows, path);

    Settings::Loot = GetPrivateProfileIntA((_("Visuals")), (_("m_loot")), Settings::Loot, path);
    Settings::LootChests = GetPrivateProfileIntA((_("Visuals")), (_("m_chests")), Settings::LootChests, path);

    Settings::Aimbot = GetPrivateProfileIntA((_("Aimbot")), (_("m_aimbot")), Settings::Aimbot, path);
    Settings::Smoothness = GetPrivateProfileIntA((_("Aimbot")), (_("m_smooth")), Settings::Smoothness, path);
    Settings::FovCircle_Value = GetPrivateProfileFloat((_("Aimbot")), (_("m_aimbotfov")), Settings::FovCircle_Value, path);
    Settings::Humanization = GetPrivateProfileFloat((_("Aimbot")), (_("m_humanization")), Settings::Humanization, path);
    Settings::IgnoreDowned = GetPrivateProfileFloat((_("Aimbot")), (_("m_ignoredowned")), Settings::IgnoreDowned, path);
    Settings::Prediction = GetPrivateProfileFloat((_("Aimbot")), (_("m_prediction")), Settings::Prediction, path);
    Settings::Headdot = GetPrivateProfileIntA((_("Aimbot")), (_("m_circleline")), Settings::Headdot, path);
    Settings::Aimkey = GetPrivateProfileIntA((_("Aimbot")), (_("m_aimkey")), Settings::Aimkey, path);
    Settings::Triggerkey = GetPrivateProfileIntA((_("Aimbot")), (_("m_triggerkey")), Settings::Triggerkey, path);
    Settings::Head = GetPrivateProfileIntA((_("Aimbot")), (_("m_head")), Settings::Head, path);
    Settings::Chest = GetPrivateProfileIntA((_("Aimbot")), (_("m_chest")), Settings::Chest, path);
    Settings::Pelvis = GetPrivateProfileIntA((_("Aimbot")), (_("m_pelvis")), Settings::Pelvis, path);
    Settings::Triggerbot = GetPrivateProfileIntA((_("Aimbot")), (_("m_triggerbot")), Settings::Triggerbot, path);
    Settings::AlwaysActive = GetPrivateProfileIntA((_("Aimbot")), (_("m_alwaysactive")), Settings::AlwaysActive, path);

    Settings::ShowFOVCircle = GetPrivateProfileIntA((_("Misc")), (_("m_showfov")), Settings::ShowFOVCircle, path);
    Settings::ShowCrosshair = GetPrivateProfileIntA((_("Misc")), (_("m_showcrosshair")), Settings::ShowCrosshair, path);
    Settings::Indicators = GetPrivateProfileIntA((_("Misc")), (_("m_indicators")), Settings::Indicators, path);
    Settings::VisibleCheck = GetPrivateProfileIntA((_("Misc")), (_("m_visiblecheck")), Settings::VisibleCheck, path);
    Settings::VSYC = GetPrivateProfileIntA((_("Misc")), (_("m_vsyc")), Settings::VSYC, path);

    Settings::MaxEspDistance = GetPrivateProfileIntA((_("Distance")), (_("m_playerdistance")), Settings::MaxEspDistance, path);
    Settings::MaxWorldDistance = GetPrivateProfileIntA((_("Distance")), (_("m_lootdistance")), Settings::MaxWorldDistance, path);
};