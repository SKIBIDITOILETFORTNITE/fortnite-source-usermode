#pragma once

static bool ShowMenu = 1;
static int tabs = 5;

ID3D11Device* g_pd3dDevice;
ID3D11DeviceContext* g_pd3dDeviceContext;
IDXGISwapChain* g_pSwapChain;
ID3D11RenderTargetView* g_mainRenderTargetView;
D3DX11_IMAGE_LOAD_INFO info;
ID3DX11ThreadPump* pump{ nullptr };
MSG Message = { NULL };
HWND MyWnd = NULL;

void SaveConfigLoop()
{
	SaveSettings((_("C:\\FN-DefaultConfig.ini")));
}

void DrawMenu()
{
	ImGui::PushFont(MenuFont);
	GaGui::Input::Handle();

	static FVector2D pos{ 300, 300 };

	if (GaGui::Window(_(""), &pos, FVector2D{ 750.f, 440.f }, ShowMenu))
	{
		GaGui::drawFilledRect(FVector2D(pos.X - 4, pos.Y), 750.f, 440.f, ImColor(0.00392, 0.00392, 0.00392, 1.0f));
		GaGui::Draw_Line(FVector2D(pos.X + 197, pos.Y), FVector2D(pos.X + 197, pos.Y + 440.f), 1.0f, ImColor(107, 107, 107, 255));
		std::wstring text = std::wstring(_(L"Astra | ")) + std::to_wstring((int)ImGui::GetIO().Framerate);
		GaGui::TextLeft(std::string(text.begin(), text.end()).c_str(), FVector2D(pos.X + 19, pos.Y + 26), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);
		GaGui::Draw_Line(FVector2D(pos.X, pos.Y + 26 + 20), FVector2D(pos.X + 197, pos.Y + 26 + 20), 1.0f, ImColor(107, 107, 107, 255));

		GaGui::PushElementY(62);
		if (GaGui::InvisibleButton(_("##aimbot"), FVector2D(172, 31))) tabs = 0;
		GaGui::TextLeft(_("Aimbot"), FVector2D(pos.X + 22, pos.Y + 69), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);

		GaGui::PushElementY(98);
		if (GaGui::InvisibleButton(_("##visuals"), FVector2D(172, 31))) tabs = 1;
		GaGui::TextLeft(_("Visuals"), FVector2D(pos.X + 22, pos.Y + 105), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);

		GaGui::PushElementY(134);
		if (GaGui::InvisibleButton(_("##world"), FVector2D(172, 31))) tabs = 2;
		GaGui::TextLeft(_("World"), FVector2D(pos.X + 22, pos.Y + 141), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);

		GaGui::PushElementY(170);
		if (GaGui::InvisibleButton(_("##triggerbot"), FVector2D(172, 31))) tabs = 3;
		GaGui::TextLeft(_("Triggerbot"), FVector2D(pos.X + 22, pos.Y + 177), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);

		GaGui::PushElementY(206);
		if (GaGui::InvisibleButton(_("##misc"), FVector2D(172, 31))) tabs = 4;
		GaGui::TextLeft(_("Miscellaneous"), FVector2D(pos.X + 22, pos.Y + 213), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);

		if (tabs == 0)
		{
			GaGui::TextLeft(_("Aimbot"), FVector2D(pos.X + 230, pos.Y + 34), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);


			GaGui::PushElementX(220);
			GaGui::PushElementY(70);
			GaGui::FakeElement();
			GaGui::Checkbox(_("Enable Aimbot"), &Settings::Aimbot);
			GaGui::Checkbox(_("Humanization"), &Settings::Humanization);
			GaGui::Checkbox(_("Prediction"), &Settings::Prediction);
			GaGui::Checkbox(_("Ignore downed"), &Settings::IgnoreDowned);
			GaGui::Checkbox(_("Circle Aim Location"), &Settings::IgnoreDowned);
			GaGui::SliderFloat(_("FOV"), &Settings::FovCircle_Value, 1.0f, 360.0f);
			GaGui::SliderInt(_("Smoothness"), &Settings::Smoothness, 1.0f, 10.0f);

			GaGui::TextLeft(_("Settings"), FVector2D(pos.X + 530, pos.Y + 34), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);

			GaGui::PushElementX(520);
			GaGui::PushElementY(70);
			GaGui::FakeElement();
			GaGui::Checkbox(_("Head"), &Settings::Head);if (Settings::Head) { Settings::Chest = false; Settings::Pelvis = false; Settings::Aimbone = 110; }
			GaGui::Checkbox(_("Chest"), &Settings::Chest);if (Settings::Chest) { Settings::Head = false; Settings::Pelvis = false; Settings::Aimbone = 37; }
			GaGui::Checkbox(_("Pelvis"), &Settings::Pelvis);if (Settings::Pelvis) { Settings::Head = false; Settings::Chest = false;Settings::Aimbone = 2; }
			GaGui::Text(_(""));
			GaGui::Text(_("Aimbot Key:"));
			GaGui::Hotkey(_("##aimkey"), FVector2D{ 110, 25 }, &Settings::Aimkey);
		}

		if (tabs == 1)
		{
			GaGui::TextLeft(_("Player"), FVector2D(pos.X + 230, pos.Y + 34), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);


			GaGui::PushElementX(220);
			GaGui::PushElementY(70);
			GaGui::FakeElement();
			GaGui::Checkbox(_("Enable Box"), &Settings::Box);
			GaGui::Checkbox(_("Enable Skeleton"), &Settings::Skeleton);
			GaGui::Checkbox(_("Enable Username"), &Settings::Nicknames);
			GaGui::Checkbox(_("Enable Platform"), &Settings::Platforms);
			GaGui::Checkbox(_("Enable Rank"), &Settings::Rank);
			GaGui::Checkbox(_("Enable Weapon"), &Settings::Weapon);
			GaGui::Checkbox(_("Enable Distance"), &Settings::Distance);
			GaGui::Checkbox(_("Enable Snapline"), &Settings::Snaplines);
			GaGui::Checkbox(_("Enable Wireframes"), &Settings::Wireframes);
			
			GaGui::Text(_("Radar Settings:"));

			GaGui::Checkbox(_("Enable Radar"), &Settings::Radar);
			GaGui::Checkbox(_("Enable Warning Arrows"), &Settings::FOVArrows);


			GaGui::TextLeft(_("Settings"), FVector2D(pos.X + 530, pos.Y + 34), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);

			GaGui::PushElementX(520);
			GaGui::PushElementY(70);
			GaGui::FakeElement();

			GaGui::SliderInt(_("Max ESP Distance"), &Settings::MaxEspDistance, 1.0f, 300.0f);
			if (Settings::Radar) {
				GaGui::SliderInt(_("Radar Size"), &Settings::RadarSize, 10.0f, 1000.0f);
			}

			GaGui::Text(_("BoxType:"));
			GaGui::Combobox("", FVector2D{ 110, 25 }, &CurrentBox, _("Corner Box"), "Bounded Box", NULL);
			if (CurrentBox == 0)
			{
				Settings::CornerBox = true;
			}
			else if (CurrentBox == 1)
			{
				Settings::CornerBox = false;
			}
		}

		if (tabs == 2)
		{
			GaGui::TextLeft(_("World"), FVector2D(pos.X + 230, pos.Y + 34), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);


			GaGui::PushElementX(220);
			GaGui::PushElementY(70);
			GaGui::FakeElement();
			GaGui::Checkbox(_("Enable Loot"), &Settings::Loot);
			GaGui::Checkbox(_("Enable Chests / Ammobox"), &Settings::LootChests);

			GaGui::TextLeft(_("Settings"), FVector2D(pos.X + 530, pos.Y + 34), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);

			GaGui::PushElementX(520);
			GaGui::PushElementY(70);
			GaGui::FakeElement();

			GaGui::SliderInt(_("Max ESP Distance"), &Settings::MaxWorldDistance, 1.0f, 300.0f);
		}
		if (tabs == 3)
		{
			GaGui::TextLeft(_("Triggerbot"), FVector2D(pos.X + 230, pos.Y + 34), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);


			GaGui::PushElementX(220);
			GaGui::PushElementY(70);
			GaGui::FakeElement();
			GaGui::Checkbox(_("Enable Triggerbot"), &Settings::Triggerbot);
			GaGui::Checkbox(_("Always Active"), &Settings::AlwaysActive);

			GaGui::TextLeft(_("Settings"), FVector2D(pos.X + 530, pos.Y + 34), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);

			GaGui::PushElementX(520);
			GaGui::PushElementY(70);
			GaGui::FakeElement();

			GaGui::Text(_("Triggerbot Key:"));
			GaGui::Hotkey(_("##aimkey"), FVector2D{ 110, 25 }, &Settings::Triggerkey);
		}
		if (tabs == 4)
		{
			GaGui::TextLeft(_("Miscellaneous"), FVector2D(pos.X + 230, pos.Y + 34), ImColor(1.0f, 1.0f, 1.0f, 1.0f), true);

			GaGui::PushElementX(220);
			GaGui::PushElementY(70);
			GaGui::FakeElement();
			GaGui::Checkbox(_("Enable FOV Circle"), &Settings::ShowFOVCircle);
			GaGui::Checkbox(_("Enable Crosshair"), &Settings::ShowCrosshair);
			GaGui::Checkbox(_("Enable Software Indicators"), &Settings::Indicators);
			GaGui::Checkbox(_("Enable Visible Check"), &Settings::VisibleCheck);
			GaGui::Checkbox(_("Enable Vsyc"), &Settings::VSYC);

			GaGui::Text(_(""));

			std::string Text = std::string(_("Thank you for purchasing")) + _(" ").decrypt() + GetPCUserName();
			GaGui::Text(Text.c_str());

		}

		if (tabs == 5)
		{
			GaGui::PushElementX(220);
			GaGui::PushElementY(70);
			GaGui::FakeElement();

			std::string Text = std::string(_("Thank you for purchasing")) + _(" ").decrypt() + GetPCUserName();
			GaGui::Text(Text.c_str());
			GaGui::Text(_("Open/Close menu is INSERT key on your keyboard"));
		}
	}

	GaGui::Render();
	ImGui::PopFont();
}

static void Shortcurts()
{
	if (Syscalls::GetAsyncKeyState(VK_INSERT) & 1)
	{
		ShowMenu = !ShowMenu;
	}
}

void render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Shortcurts();

	ProcessLoop();

	DrawMenu();
	GaGui::Draw_Cursor(ShowMenu);

	ImVec4 clear_color = ImVec4(0., 0., 0., 0.);
	ImGui::Render();
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	SetWindowPos(MyWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	if (Settings::VSYC)
		g_pSwapChain->Present(1, 0);
	else
		g_pSwapChain->Present(0, 0);
}