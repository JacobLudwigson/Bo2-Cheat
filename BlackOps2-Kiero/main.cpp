#include "includes.h"
#include "memory.h"
#include "globalVariables.h"
#include "ColorConsole.h"
#include "config.h"
#include "menu.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	// Init ImGui Hook
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	// Required ImGui stuff.
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	// Condensing all the draw menu into its own function.
	// Just looks better and easier.
	// Menu.h
	DrawMenu();


	// ImGui Frame Render
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Calling original Present function.
	// Present is the equivalent to SwapBuffers. It is the function which swaps the frame onto your screen.
	return oPresent(pSwapChain, SyncInterval, Flags);
}



DWORD WINAPI MainThread(LPVOID lpReserved)
{
	// Console
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Init DX11 Present Hook.
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			// 8 is the index of the Present function in the VTable.
			// OPresent is going to be the original present function. We can call OPresent to call original present function without our hook.
			// hkPresent is our hook where we draw GUI and do hack stuff.
			kiero::bind(8, (void**)& oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);


	Config.BaseAddress = (uintptr_t)GetModuleHandle(NULL);

	CC::DarkGreen(hConsole);
	CurrentTime = time(NULL);
	Console::PrintTime(CurrentTime);
	printf(" - Black Ops 2 Multiplayer Base Address: %X\n", Config.BaseAddress);
	CC::White(hConsole);

	Config.PID = Memory::GetProcId(L"t6mp.exe");

	CC::DarkGreen(hConsole);
	CurrentTime = time(NULL);
	Console::PrintTime(CurrentTime);
	printf(" - Black Ops 2 Multiplayer PID: %u\n", Config.PID);
	CC::White(hConsole);

	// Loop Checking if Exit Key Pressed
	while (!GetAsyncKeyState(VK_END) & 1)
	{

	}
	kiero::shutdown();
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(g_hModule, 0);
	return TRUE;
}


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}