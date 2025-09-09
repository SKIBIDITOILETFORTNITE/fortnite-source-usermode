#pragma once

void MoveTo(float x, float y) {
	float ScreenCenterX = (HUD_Info::Width / 2);
	float ScreenCenterY = (HUD_Info::Height / 2);
	int Smooth = Settings::Smoothness;
	float TargetX = 0;
	float TargetY = 0;

	if (x != 0) {
		if (x > ScreenCenterX) {
			TargetX = -(ScreenCenterX - x);
			TargetX /= Smooth;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX) {
			TargetX = x - ScreenCenterX;
			TargetX /= Smooth;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (y != 0) {
		if (y > ScreenCenterY) {
			TargetY = -(ScreenCenterY - y);
			TargetY /= Smooth;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY) {
			TargetY = y - ScreenCenterY;
			TargetY /= Smooth;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	if (Settings::Humanization) {
		int SleepTime = 1 + (std::rand() % 3);
		std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
	}

	Syscalls::Move_Mouse(TargetX, TargetY);

	return;
}

FVector Prediction(FVector TargetPosition, FVector ComponentVelocity, float PlayerDistance, float ProjectileSpeed, float Gravity)
{
	float TimeToTarget = PlayerDistance / ProjectileSpeed;
	float BulletDrop = abs(Gravity) * (TimeToTarget * TimeToTarget) * 0.5;
	return FVector
	{
	TargetPosition.X + TimeToTarget * ComponentVelocity.X,
	TargetPosition.Y + TimeToTarget * ComponentVelocity.Y,
	TargetPosition.Z + TimeToTarget * ComponentVelocity.Z + BulletDrop
	};
}

void AimbotLoop()
{
	if (Cache::ClosestPawn && (Cache::ClosestPawn != Addresses::LocalPawn)) {
		uintptr_t LocalTeamIndex = Addresses::LocalPawn->PlayerState()->TeamIndex();
		uintptr_t CurrentPawnTeamIndex = Cache::ClosestPawn->PlayerState()->TeamIndex();
		auto ProjectileSpeed = Addresses::LocalPawn->CurrentWeapon()->GetProjectileSpeed();
		auto ProjectileGravity = Addresses::LocalPawn->CurrentWeapon()->GetProjectileGravity();
		auto Velocity = Cache::ClosestPawn->RootComponent()->Velocity();
		double Distance = Cache::LocalRelativeLocation.distance(Cache::ClosestPawn->Mesh()->GetBone(Settings::Aimbone));
		auto TargetBone = Addresses::PlayerController->WorldToScreen(Cache::ClosestPawn->Mesh()->GetBone(Settings::Aimbone));
		auto Predictor = Prediction(Cache::ClosestPawn->Mesh()->GetBone(Settings::Aimbone), Velocity, Distance, ProjectileSpeed, ProjectileGravity);
		auto PredictorW2S = Addresses::PlayerController->WorldToScreen(Predictor);
		auto WeaponName = Addresses::LocalPawn->CurrentWeapon()->WeaponData()->WeaponName();
		if (LocalTeamIndex != CurrentPawnTeamIndex) {
			if (Syscalls::GetAsyncKeyState(Settings::Aimkey))
			{
				if (Settings::Headdot)
				{
					FVector HeadNoW2S = Cache::ClosestPawn->Mesh()->GetBone(Settings::Aimbone);
					FVector2D Head = Addresses::PlayerController->WorldToScreen(FVector{ HeadNoW2S.X, HeadNoW2S.Y, HeadNoW2S.Z });
					FVector Delta = HeadNoW2S - Addresses::Location;

					float distance = Delta.Length();
					const float constant_circle_size = 10;
					float circle_radius = constant_circle_size * (HUD_Info::Height / (2.0f * distance * math::custom_tanf(Addresses::FOVAngle * (float)M_PI / 360.f)));
					float y_offset = +12.0f;
					int segments = 50;

					ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(Head.X, Head.Y), circle_radius, ImColor(0, 0, 0, 255), segments, 1 + 2.0f);
					ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(Head.X, Head.Y), circle_radius, ImColor(255, 0, 0, 255), segments, 1);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(HUD_Info::Width / 2, HUD_Info::Height / 2), ImVec2(Head.X, Head.Y), ImColor(0, 0, 0, 255), 1 + 2.0f);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(HUD_Info::Width / 2, HUD_Info::Height / 2), ImVec2(Head.X, Head.Y), ImColor(255, 0, 0, 255), 1);
				}

				if (strstr(WeaponName.c_str(), _("Sniper")) || strstr(WeaponName.c_str(), _("Hunting")) || strstr(WeaponName.c_str(), _("DMR")))
					MoveTo(PredictorW2S.X, PredictorW2S.Y);
				else
					MoveTo(TargetBone.X, TargetBone.Y);
			}
		}
	}
}