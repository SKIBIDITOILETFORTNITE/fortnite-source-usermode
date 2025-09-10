#pragma once

void CreateTextureFromMemory(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	const void* imageData,
	size_t imageDataSize,
	UINT width,
	UINT height,
	DXGI_FORMAT format,
	ID3D11ShaderResourceView** textureView
) {
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = imageData;
	initData.SysMemPitch = static_cast<UINT>(width * 4);
	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = device->CreateTexture2D(&textureDesc, &initData, &texture);
	if (FAILED(hr)) {
		return;
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, textureView);
	texture->Release();
	if (FAILED(hr)) {
		return;
	}
}

bool DirectXSetup() {
	DXGI_SWAP_CHAIN_DESC swap_chain_description;
	ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));
	swap_chain_description.BufferCount = 2;
	swap_chain_description.BufferDesc.Width = 0;
	swap_chain_description.BufferDesc.Height = 0;
	swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.OutputWindow = MyWnd;
	swap_chain_description.SampleDesc.Count = 1;
	swap_chain_description.SampleDesc.Quality = 0;
	swap_chain_description.Windowed = 1;
	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL d3d_feature_lvl;

	const D3D_FEATURE_LEVEL d3d_feature_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, d3d_feature_array, 2, D3D11_SDK_VERSION, &swap_chain_description, &g_pSwapChain, &g_pd3dDevice, &d3d_feature_lvl, &g_pd3dDeviceContext);

	ID3D11Texture2D* pBackBuffer;
	D3DX11_IMAGE_LOAD_INFO info;
	ID3DX11ThreadPump* pump{ nullptr };

	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);

	pBackBuffer->Release();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;

	ImFontConfig CustomFont;
	CustomFont.FontDataOwnedByAtlas = false;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;
	ImGui_ImplWin32_Init(MyWnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	ImFontConfig font_config;
	font_config.OversampleH = 1;
	font_config.OversampleV = 1;
	font_config.PixelSnapH = 1;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF,
		0x0400, 0x044F,
		0,
	};

	ESPFont = io.Fonts->AddFontFromMemoryTTF(RobotoData, sizeof(RobotoData), 14.0f);
	MenuFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanab.ttf", 12.f);
	if (Unranked == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, UnrankedImg, sizeof(UnrankedImg), &info, pump, &Unranked, 0);
	if (Bronze == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, BronzeImg, sizeof(BronzeImg), &info, pump, &Bronze, 0);
	if (Silver == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, SilverImg, sizeof(SilverImg), &info, pump, &Silver, 0);
	if (Gold == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, GoldImg, sizeof(GoldImg), &info, pump, &Gold, 0);
	if (Platinum == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, PlatinumImg, sizeof(PlatinumImg), &info, pump, &Platinum, 0);
	if (Diamond == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, DiamondImg, sizeof(DiamondImg), &info, pump, &Diamond, 0);
	if (Elite == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, EliteImg, sizeof(EliteImg), &info, pump, &Elite, 0);
	if (Champion == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, ChampionImg, sizeof(ChampionImg), &info, pump, &Champion, 0);
	if (Unreal == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, UnrealImg, sizeof(UnrealImg), &info, pump, &Unreal, 0);

	ImGui_ImplWin32_Init(MyWnd);

	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	g_pd3dDevice->Release();

	return true;
}

bool SetupWindow()
{
	while (!MyWnd) {
		MyWnd = SetUp::HiJackNotepadWindow();
		Sleep(10);
	}
	MARGINS margin = { -2 };
	DwmExtendFrameIntoClientArea(MyWnd, &margin);
	SetMenu(MyWnd, NULL);
	CustomSetWindowLongA(MyWnd, GWL_STYLE, WS_VISIBLE);
	CustomSetWindowLongA(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
	CustomShowWindow(MyWnd, SW_SHOW);
	CustomUpdateWindow(MyWnd);
	CustomSetWindowLongA(MyWnd, GWL_EXSTYLE, GetWindowLong(MyWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
	return true;
}

bool MainLoop() {
	static IDXGISwapChain* pSwapChain;
	IDXGISwapChain* pSwaChain = nullptr;
	static RECT rect_og;
	MSG msg = { NULL };
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		CustomUpdateWindow(MyWnd);
		CustomShowWindow(MyWnd, SW_SHOW);

		if (CustomPeekMessageA(&msg, MyWnd, 0, 0, PM_REMOVE))
		{
			CustomTranslateMessage(&msg);
			CustomDispatchMessageA(&msg);
		}

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = MyWnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p_cursor;
		CustomGetCursorPosA(&p_cursor);
		io.MousePos.x = p_cursor.x;
		io.MousePos.y = p_cursor.y;

		if (Syscalls::GetAsyncKeyState(VK_LBUTTON)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;

		render();
	}
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	DestroyWindow(MyWnd);


	return true;
}