#pragma once

__forceinline void draw_line(int x1, int y1, int x2, int y2, int thickness, ImColor clr) {
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), clr, thickness);
}

__forceinline void draw_corner_box(int x, int y, int w, int h, int thickness, ImColor clr) {
	draw_line(x, y, x, y + (h / 3), thickness, clr);
	draw_line(x, y, x + (w / 3), y, thickness, clr);
	draw_line(x + w, y, x + w - (w / 3), y, thickness, clr);
	draw_line(x + w, y, x + w, y + (h / 3), thickness, clr);
	draw_line(x, y + h, x + (w / 3), y + h, thickness, clr);
	draw_line(x, y + h, x, y + h - (h / 3), thickness, clr);
	draw_line(x + w, y + h, x + w - (w / 3), y + h, thickness, clr);
	draw_line(x + w, y + h, x + w, y + h - (h / 3), thickness, clr);
}

inline void draw_bounding_box(int x, int y, float w, float h, int thickness, ImColor color)
{
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, thickness);
}

inline void ESPLoop(AFortPawn* CurrentPawn, FVector2D Head, FVector2D Root, bool IsVisible)
{
	uintptr_t LocalTeamIndex = Addresses::LocalPawn->PlayerState()->TeamIndex();
	uintptr_t CurrentPawnTeamIndex = CurrentPawn->PlayerState()->TeamIndex();

	ImColor Color = ImColor(255, 255, 255, 255);
	ImColor SkeletonColor = ImColor(255, 255, 255, 255);
	if (IsVisible) {
		Color = ImColor(255, 255, 255, 255);
		SkeletonColor = ImColor(0, 255, 191, 255);
	}
	else {
		Color = ImColor(255, 0, 0, 255);
		SkeletonColor = ImColor(255, 0, 0, 255);
	}
	if (LocalTeamIndex == CurrentPawnTeamIndex) {
		Color = ImColor(0, 255, 0, 255);
		SkeletonColor = ImColor(0, 255, 0, 255);
	}

	FVector WorldHead = CurrentPawn->Mesh()->GetBone(110);
	FVector2D Top = Addresses::PlayerController->WorldToScreen({ WorldHead.X, WorldHead.Y, WorldHead.Z + 20 });
	FVector2D Neck = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(67));
	FVector2D Chest = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(7));
	FVector2D Pelvis = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(2));
	FVector2D RightShoulder = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(9));
	FVector2D RightElbow = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(10));
	FVector2D RightWrist = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(11));
	FVector2D LeftShoulder = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(38));
	FVector2D LeftElbow = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(39));
	FVector2D LeftWrist = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(40));
	FVector2D RightHip = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(71));
	FVector2D RightKnee = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(72));
	FVector2D RightAnkle = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(73));
	FVector2D RightFootUpper = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(86));
	FVector2D LeftHip = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(78));
	FVector2D LeftKnee = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(79));
	FVector2D LeftAnkle = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(80));
	FVector2D LeftFootUpper = Addresses::PlayerController->WorldToScreen(CurrentPawn->Mesh()->GetBone(87));
	FVector WorldRoot = CurrentPawn->Mesh()->GetBone(0);
	FVector2D Bottom = Addresses::PlayerController->WorldToScreen({ WorldRoot.X, WorldRoot.Y, WorldRoot.Z + 5 });

	float BoxHeight = Root.Y - Top.Y;
	float BoxWidth = (abs(Top.Y - Root.Y)) * 0.40f;
	float OffsetTop = 10.f;
	float OffsetBottom = 10.f;
	FBounds Bounds = CurrentPawn->ActorBounds(Top, Root, BoxWidth);

	if (FN::IsInScreen(Head) && (CurrentPawn != Addresses::LocalPawn))
	{
		ImGui::PushFont(ESPFont);
		if (Settings::Box)
		{
			if (!Settings::CornerBox) {
				draw_bounding_box(Bounds.Left, Bounds.Top, Bounds.Right - Bounds.Left, Bounds.Bottom - Bounds.Top, 3, ImColor(0, 0, 0, 255));
				draw_bounding_box(Bounds.Left, Bounds.Top, Bounds.Right - Bounds.Left, Bounds.Bottom - Bounds.Top, 1, Color);
			}
			else {
				draw_corner_box(Bounds.Left, Bounds.Top, Bounds.Right - Bounds.Left, Bounds.Bottom - Bounds.Top, 2, ImColor(0, 0, 0, 255));
				draw_corner_box(Bounds.Left, Bounds.Top, Bounds.Right - Bounds.Left, Bounds.Bottom - Bounds.Top, 1, Color);
			}
		}
		if (Settings::Skeleton)
		{
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Neck.X, Neck.Y), ImVec2(Chest.X, Chest.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Chest.X, Chest.Y), ImVec2(Pelvis.X, Pelvis.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Chest.X, Chest.Y), ImVec2(RightShoulder.X, RightShoulder.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(RightShoulder.X, RightShoulder.Y), ImVec2(RightElbow.X, RightElbow.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(RightElbow.X, RightElbow.Y), ImVec2(RightWrist.X, RightWrist.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Chest.X, Chest.Y), ImVec2(LeftShoulder.X, LeftShoulder.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(LeftShoulder.X, LeftShoulder.Y), ImVec2(LeftElbow.X, LeftElbow.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(LeftElbow.X, LeftElbow.Y), ImVec2(LeftWrist.X, LeftWrist.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Pelvis.X, Pelvis.Y), ImVec2(RightHip.X, RightHip.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(RightHip.X, RightHip.Y), ImVec2(RightKnee.X, RightKnee.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(RightKnee.X, RightKnee.Y), ImVec2(RightAnkle.X, RightAnkle.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(RightAnkle.X, RightAnkle.Y), ImVec2(RightFootUpper.X, RightFootUpper.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Pelvis.X, Pelvis.Y), ImVec2(LeftHip.X, LeftHip.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(LeftHip.X, LeftHip.Y), ImVec2(LeftKnee.X, LeftKnee.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(LeftKnee.X, LeftKnee.Y), ImVec2(LeftAnkle.X, LeftAnkle.Y), SkeletonColor, 1.0f);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(LeftAnkle.X, LeftAnkle.Y), ImVec2(LeftFootUpper.X, LeftFootUpper.Y), SkeletonColor, 1.0f);
		}
		if (Settings::Nicknames)
		{
			std::string Text = CurrentPawn->PlayerState()->GetPlayerName();
			ImVec2 TextSize = ImGui::CalcTextSize(Text.c_str());
			ImVec2 pos = ImVec2(Top.X - TextSize.x / 2, Top.Y - OffsetTop - TextSize.y / 2);
			DrawString(14, pos, GammaCorrectedColor(255, 255, 255), false, true, Text.c_str());

			OffsetTop += 15;
		}

		if (Settings::Platforms)
		{
			auto Platform = CurrentPawn->PlayerState()->GetPlatform();
			std::string Text;
			ImColor PlatformColor = GammaCorrectedColor(255, 0, 0);
			if (Platform == _("PSN").decrypt())
				Text = _("Playstation");
			else if (Platform == _("PS5").decrypt())
				Text = _("Playstation 5");
			else if (Platform == _("XBL").decrypt())
				Text = _("Xbox");
			else if (Platform == _("XSX").decrypt())
				Text = _("Xbox Series S");
			else if (Platform == _("WIN").decrypt())
				Text = _("Windows");
			else if (Platform == _("MAC").decrypt())
				Text = _("MacOs");
			else if (Platform == _("IOS").decrypt())
				Text = _("IOS");
			else if (Platform == _("AND").decrypt())
				Text = _("Android");
			else if (Platform == _("SWT").decrypt())
				Text = _("NintendoSwitch");
			else
				Text = _("UNAVALIBLE");

			ImVec2 TextSize = ImGui::CalcTextSize(Text.c_str());
			ImVec2 pos = ImVec2(Top.X - TextSize.x / 2, Top.Y - OffsetTop - TextSize.y / 2);
			DrawString(14, pos, PlatformColor, false, true, Text.c_str());

			OffsetTop += 30;
		}

		if (Settings::Rank)
		{
			auto RankTier = CurrentPawn->PlayerState()->GetRank();
			std::string Text = _("").decrypt();;
			ImVec2 TextSize = ImGui::CalcTextSize(Text.c_str());
			ImVec2 DrawPos = ImVec2(Top.X - 15 - TextSize.x / 2, Top.Y - OffsetTop - TextSize.y / 2);
			//ImVec2 DrawPos = ImVec2(Head.X - 15, Head.Y - OffsetTop);
			if (RankTier == 0) //Bronze I
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Bronze), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 1) //Bronze II
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Bronze), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 2) //Bronze III
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Bronze), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 3) //Silver I
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Silver), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 4) //Silver II
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Silver), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 5) //Silver III
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Silver), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 6) //Gold I
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Gold), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 7) //Gold II
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Gold), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 8) //Gold III
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Gold), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 9) //Platinum I
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Platinum), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 10) //Platinum II
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Platinum), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 11) //Platinum III
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Platinum), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 12) //Diamond I
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Diamond), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 13) //Diamond II
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Diamond), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 14) //Diamond III
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Diamond), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 15) //Elite
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Elite), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 16) //Champion
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Champion), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else if (RankTier == 17) //Unreal
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Unreal), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));
			else //Unranked
				ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(Unranked), DrawPos, ImVec2(DrawPos.x + 30, DrawPos.y + 30));

			OffsetTop += 15;
		}

		if (Settings::Weapon)
		{
			auto CurrentWeapon = CurrentPawn->CurrentWeapon();
			if (CurrentWeapon) {
				auto AmmoCount = CurrentWeapon->AmmoCount();
				auto WeaponData = CurrentWeapon->WeaponData();
				if (WeaponData) {
					std::string GetCurrentWeapon = WeaponData->WeaponName();
					std::string DisplayText;
					if (GetCurrentWeapon.empty())
						DisplayText = _("Building");
					else {
						DisplayText = GetCurrentWeapon + _(" [").decrypt() + std::to_string(AmmoCount) + _("]").decrypt();
					}
					auto Tier = CurrentPawn->CurrentWeapon()->WeaponData()->Rarity();
					ImVec2 TextSize = ImGui::CalcTextSize(DisplayText.c_str());
					ImVec2 pos = ImVec2(Bottom.X - TextSize.x / 2, Bottom.Y + OffsetBottom - TextSize.y / 2);
					DrawString(14, pos, GetColorFromTier(Tier), false, true, DisplayText.c_str());

					OffsetBottom += 15;
				}
			}
		}

		if (Settings::Distance)
		{
			double Distance;
			if (Addresses::LocalPawn)
				Distance = Cache::LocalRelativeLocation.distance(CurrentPawn->RootComponent()->RelativeLocation()) / 100.0f;
			else
				Distance = Addresses::Location.distance(CurrentPawn->RootComponent()->RelativeLocation()) / 100.0f;
			char dist[64];
			sprintf_s(dist, "[ %.fm ]", Distance);
			ImVec2 TextSize = ImGui::CalcTextSize(dist);
			ImVec2 pos = ImVec2(Bottom.X - TextSize.x / 2, Bottom.Y + OffsetBottom - TextSize.y / 2);
			DrawString(14, pos, ImColor(255, 255, 255), false, true, dist);

			OffsetBottom += 15;
		}

		if (Settings::Snaplines)
		{
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(HUD_Info::Width / 2, HUD_Info::Height), ImVec2(Root.X, Root.Y), Color, 1);
		}
		ImGui::PopFont();
	}

	if (Settings::FOVArrows)
	{
		if (CurrentPawn != Addresses::LocalPawn) {
			FVector vAngle = Addresses::Rotation;
			float fYaw = vAngle.Y * M_PI / 180.0f;
			float dx = CurrentPawn->RootComponent()->RelativeLocation().X - Addresses::Location.X;
			float dy = CurrentPawn->RootComponent()->RelativeLocation().Y - Addresses::Location.Y;
			float fsin_yaw = math::custom_sinf(fYaw);
			float fminus_cos_yaw = -math::custom_cosf(fYaw);
			float x = dy * fminus_cos_yaw + dx * fsin_yaw;
			x = -x;
			float y = dx * fminus_cos_yaw - dy * fsin_yaw;
			float length = math::custom_sqrtf(x * x + y * y);
			x /= length;
			y /= length;
			float angle = math::custom_atan2f(y, x);
			float fov_radius;
			fov_radius = (Settings::FovCircle_Value * HUD_Info::Width / (double)Addresses::FOVAngle) / 2 + 5.0f;
			float screen_center_x = HUD_Info::Width / 2;
			float screen_center_y = HUD_Info::Height / 2;
			FVector2D arrow_pos = { screen_center_x + x * fov_radius, screen_center_y + y * fov_radius };
			DrawArrow(arrow_pos.X, arrow_pos.Y, angle, 16.0f, ImColor(255, 0, 0, 255));
		}
	}
}