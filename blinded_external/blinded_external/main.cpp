#include "Includes.h"

auto main() -> int {

	Start();
	Sleep(500);
	if (!Driver::Init())
	{
		
		Beep(500, 500);

	}

	CustomInterface();

	HUD_Info::Width = CustomGetSystemMetrics(SM_CXSCREEN);
	HUD_Info::Height = CustomGetSystemMetrics(SM_CYSCREEN);

	Driver::ProcessID = Driver::FindProcess(L"FortniteClient-Win64-Shipping.exe");
	if (!Driver::ProcessID) {
		Beep(500, 500);
		MessageBoxA(0, _("Please ensure game is open, or restart device"), _("Process Not Found"), MB_OK | MB_ICONERROR);
		exit(1);
	}

	Addresses::base = Driver::GetBase();
	if (!Addresses::base) {
		Beep(500, 500);
		MessageBoxA(0, _("Please ensure game is open, or restart device"), _("Failed To Get Image"), MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (!Driver::CR3())
	{
		Beep(500, 500);
		MessageBoxA(0, _("Please ensure game is open, or restart device"), _("Failed To Get Directory Base"), MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (!Syscalls::Init()) {
		Beep(500, 500);
		MessageBoxA(0, _("Please ensure game is open, or restart device"), _("Mouse Hijack Failure"), MB_OK | MB_ICONERROR);
		exit(1);
	}

	LoadSettings((_("C:\\FN-DefaultConfig.ini")));

	printf(_("\n Successfully Started Without Error, Enjoy!"));

	std::thread([]() { while (true) { SaveConfigLoop(); std::this_thread::sleep_for(std::chrono::milliseconds(10)); } }).detach();
	std::thread([]() { while (true) { WorldCache(); std::this_thread::sleep_for(std::chrono::milliseconds(10)); } }).detach();
	std::thread([]() { while (true) { ActorCache(); std::this_thread::sleep_for(std::chrono::milliseconds(10)); } }).detach();

	SetupWindow();
	DirectXSetup();
	MainLoop();
}