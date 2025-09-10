#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <algorithm>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <dwmapi.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mutex>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <random>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <cstdlib>
#include <algorithm>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "D3DX11tex.h"
#include <d3d11.h>
#include <dxgi.h>
#include <Dwmapi.h>

#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"
#include "Bytes.hpp"
#include "Images.hpp"

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

void Start();

ImFont* MenuFont;
ImFont* ESPFont;
ID3D11ShaderResourceView* Unranked = nullptr;
ID3D11ShaderResourceView* Bronze = nullptr;
ID3D11ShaderResourceView* Silver = nullptr;
ID3D11ShaderResourceView* Gold = nullptr;
ID3D11ShaderResourceView* Platinum = nullptr;
ID3D11ShaderResourceView* Diamond = nullptr;
ID3D11ShaderResourceView* Elite = nullptr;
ID3D11ShaderResourceView* Champion = nullptr;
ID3D11ShaderResourceView* Unreal = nullptr;

#include "crypt.h"
#include "Lazy.h"
#include "driver.hpp"
#include "mouse.h"
#include "custom_math.h"
#include "Spoof.hpp"
#include "Structs.h"
#include "Enums.h"
#include "Settings.h"
#include "HijackWindow.h"
#include "Config.h"
#include "Offsets.h"
#include "API.h"
#include "CustomFramework.h"
#include "FName.h"
#include "Radar.h"
#include "Aimbot.h"
#include "TriggerBot.h"
#include "Esp.h"
#include "World.h"
#include "Menu.h"
#include "Overlay.h"