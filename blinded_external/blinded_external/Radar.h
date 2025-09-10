#pragma once

void RadarRange(float* x, float* y, float range)
{
	if (crt::c_fabs((*x)) > range || crt::c_fabs((*y)) > range)
	{
		if ((*y) > (*x))
		{
			if ((*y) > -(*x))
			{
				(*x) = range * (*x) / (*y);
				(*y) = range;
			}
			else
			{
				(*y) = -range * (*y) / (*x);
				(*x) = -range;
			}
		}
		else
		{
			if ((*y) > -(*x))
			{
				(*y) = range * (*y) / (*x);
				(*x) = range;
			}
			else
			{
				(*x) = -range * (*x) / (*y);
				(*y) = -range;
			}
		}
	}
}

void CalcRadarPoint(FVector vOrigin, int& screenx, int& screeny)
{
	FVector vAngle = FVector{ Addresses::Rotation.X, Addresses::Rotation.Y, Addresses::Rotation.Z };
	auto fYaw = vAngle.Y * M_PI / 180.0f;
	float dx = vOrigin.X - Addresses::Location.X;
	float dy = vOrigin.Y - Addresses::Location.Y;

	float fsin_yaw = math::custom_sinf(fYaw);
	float fminus_cos_yaw = -math::custom_cosf(fYaw);

	float x = dy * fminus_cos_yaw + dx * fsin_yaw;
	x = -x;
	float y = dx * fminus_cos_yaw - dy * fsin_yaw;

	float range = (float)Settings::RadarDistance * 10;

	RadarRange(&x, &y, range);

	ImVec2 DrawPos = ImGui::GetCursorScreenPos();
	ImVec2 DrawSize = ImGui::GetContentRegionAvail();

	int rad_x = (int)DrawPos.x;
	int rad_y = (int)DrawPos.y;

	float r_siz_x = DrawSize.x;
	float r_siz_y = DrawSize.y;

	int x_max = (int)r_siz_x + rad_x - 5;
	int y_max = (int)r_siz_y + rad_y - 5;

	screenx = rad_x + ((int)r_siz_x / 2 + int(x / range * r_siz_x));
	screeny = rad_y + ((int)r_siz_y / 2 + int(y / range * r_siz_y));

	if (screenx > x_max)
		screenx = x_max;

	if (screenx < rad_x)
		screenx = rad_x;

	if (screeny > y_max)
		screeny = y_max;

	if (screeny < rad_y)
		screeny = rad_y;
}

inline void renderRadar(AFortPawn* Pawn) {
	int screenx = 0;
	int screeny = 0;

	float Color_R = 255;
	float Color_G = 0;
	float Color_B = 0;
	float Color_A = 255;

	auto pos = Pawn->RootComponent()->RelativeLocation();

	CalcRadarPoint(pos, screenx, screeny);

	ImDrawList* Draw = ImGui::GetForegroundDrawList();

	if (Pawn->Mesh()->IsVisible()) {
		Color_R = 0;
		Color_G = 255;
		Color_B = 0;
	}

	uintptr_t LocalTeamIndex = Addresses::LocalPawn->PlayerState()->TeamIndex();
	uintptr_t CurrentPawnTeamIndex = Pawn->PlayerState()->TeamIndex();

	if (CurrentPawnTeamIndex == LocalTeamIndex)
	{
		Color_R = 173;
		Color_G = 127;
		Color_B = 0;
	}

	Draw->AddRectFilled(ImVec2((float)screenx, (float)screeny),
		ImVec2((float)screenx + 5, (float)screeny + 5),
		ImColor(Color_R, Color_G, Color_B, Color_A));

}

inline void Initialize_RADAR(AFortPawn* Pawn)
{
	if (Settings::Radar) {
		ImGui::StyleColorsDark();
		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 1.0f;
		style->FramePadding = ImVec2(5, 5);
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 0.0f;
		style->ScrollbarRounding = 1.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;
		style->Alpha = 1.f;
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImColor(25, 25, 25, 180);
		colors[ImGuiCol_FrameBg] = ImColor(25, 25, 25, 180);
		colors[ImGuiCol_FrameBgHovered] = ImColor(25, 25, 25, 180);
		colors[ImGuiCol_FrameBgActive] = ImColor(25, 25, 25, 180);
		ImGui::SetNextWindowPos(ImVec2{ (float)Settings::RadarX, (float)Settings::RadarY }, ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(Settings::RadarSize, Settings::RadarSize));
		static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;
		if (ImGui::Begin(_("RADAR"), nullptr, flags)) {

			ImDrawList* Draw = ImGui::GetForegroundDrawList();
			ImVec2 DrawPos = ImGui::GetCursorScreenPos();
			ImVec2 DrawSize = ImGui::GetContentRegionAvail();

			ImVec2 midRadar = ImVec2(DrawPos.x + (DrawSize.x / 2), DrawPos.y + (DrawSize.y / 2));

			ImGui::GetWindowDrawList()->AddLine(ImVec2(midRadar.x - DrawSize.x / 2.f, midRadar.y), ImVec2(midRadar.x + DrawSize.x / 2.f, midRadar.y), ImColor(50, 50, 50, 50));
			ImGui::GetWindowDrawList()->AddLine(ImVec2(midRadar.x, midRadar.y - DrawSize.y / 2.f), ImVec2(midRadar.x, midRadar.y + DrawSize.y / 2.f), ImColor(50, 50, 50, 50));

			renderRadar(Pawn);

		}
		ImGui::End();
	}
}