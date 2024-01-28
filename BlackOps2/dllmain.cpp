// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "globalVariables.h"

DWORD WINAPI MainThread(LPVOID lpReserved)
{
    // Console Allocation
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int i = 0;
    // Main Loop
    while (!GetAsyncKeyState(VK_END) & 1)
    {
        CC::CustomColor(hConsole, i);
        if (i > 255)
            i = 0;
        else
            i++;
        CurrentTime = time(NULL);
        Console::PrintTime(CurrentTime);
        printf(" - Seizureeeeeeee.\n");
    }

    // Console Cleanup
    fclose(f);
    FreeConsole();
    // DLL Cleanup
    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    g_hModule = hModule;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(g_hModule);
        CreateThread(nullptr, 0, MainThread, g_hModule, 0, nullptr);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

