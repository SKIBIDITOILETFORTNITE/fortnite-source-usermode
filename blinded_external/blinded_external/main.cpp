#include "Includes.h"




HWND fortnite_window;


auto main() -> int {

    Start();
	Sleep(500);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


    if (!driver::FindDriverHandle())
    {
        std::cout << ("\n  [+] ");
        std::cout << ("Driver not found. Loading driver... ");

        Sleep(1000);
        if (!driver::FindDriverHandle())
        {
            system("cls");
            std::cout << ("\n \n \n  [!] ");
            std::cout << ("Failed to load driver. Please restart your PC or contact support. ");
            Sleep(3000);
            exit(1);
        }
        std::cout << ("\n  [+] ");
        std::cout << ("Driver loaded successfully! ");
        Sleep(1000);
    }
    else
    {
        std::cout << ("\n  [+] ");
        std::cout << ("Driver already loaded. Proceeding... ");
        Sleep(1000);
    }

    if (!driver::FindDriverHandle())
    {
        INT32 fortnite_pid = driver::FindProcess(L"FortniteClient-Win64-Shipping.exe");
        if (fortnite_pid)
        {
            std::cout << ("  [!] ");
            std::cout << ("Close Fortnite first!");
            while (driver::FindProcess(L"FortniteClient-Win64-Shipping.exe")) {
                Sleep(500);
            }
            std::cout << ("  [+] ");
            std::cout << ("Thanks! Loading... ");
            Sleep(3500);
        }

        Sleep(2000);
        if (!driver::FindDriverHandle())
        {
            system("cls");
            SetConsoleTextAttribute(hConsole, 4);
            std::cout << ("\n \n \n  [!] ");
            SetConsoleTextAttribute(hConsole, 7);
            std::cout << ("Failed to load. Restart your PC! Contact support if you get this again after the restart. ");
            Sleep(3000);
            exit(1);
        }
    }

    system("cls");
    std::cout << ("\n\n\n  [+] ");
    SetConsoleTextAttribute(hConsole, 7);
    std::cout << ("Open Fortnite! ");

    HUD_Info::Width = GetSystemMetrics(SM_CXSCREEN);
    HUD_Info::Height = GetSystemMetrics(SM_CYSCREEN);

    while (true)
    {
        INT32 fortnite_pid = driver::FindProcess(L"FortniteClient-Win64-Shipping.exe");
        if (fortnite_pid)
        {
            driver::process_id = fortnite_pid;
            break;
        }
        Sleep(500);
    }

    while (fortnite_window == NULL)
    {
        fortnite_window = FindWindowW(L"UnrealWindow", L"Fortnite  ");
        Sleep(500);
    }

    std::cout << ("\n  [+] ");
    std::cout << ("Fortnite Found!");
    std::cout << ("\n  [+] ");
    std::cout << ("Press [ENTER] in Lobby!");
    while (!(GetAsyncKeyState(VK_RETURN) & 0x8000)) {
        Sleep(10);
    }

    std::cout << ("\n\n\n  [+] ");
    std::cout << ("Loading... ");

    driver::base_address = driver::FindImage();
    if (driver::base_address == 0)
    {
        system("cls");
        std::cout << ("\n \n \n  [!] ");
        std::cout << ("Failed to get base address for Fortnite. Error: 0x0002 ");
        Sleep(2000);
        exit(1);
    }

    uintptr_t cr3 = driver::DecryptCr3();
    if (cr3 == 0)
    {
        system("cls");
        std::cout << ("\n \n \n  [!] ");
        std::cout << ("Failed to retrieve CR3 for Fortnite. Error: 0x0003 ");
        Sleep(2000);
        exit(1);
    }


	CustomInterface();

	HUD_Info::Width = CustomGetSystemMetrics(SM_CXSCREEN);
	HUD_Info::Height = CustomGetSystemMetrics(SM_CYSCREEN);

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