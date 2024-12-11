#include "pch.h"
#include "Windows.h"
#include <string>
#include "Globals.h"
#include "injector/injector.hpp"
#include "ini.h"

DWORD GameState;
float tod = 0;
int keyInc, keyDec;
char(__cdecl* SetTimeDay)(float a1) = (char(__cdecl*)(float))0x7696B0;

void Main();
DWORD WINAPI PointerKey(LPVOID);

void Main()
{
    mINI::INIFile file("TimeCycle.ini");
    mINI::INIStructure ini;
    file.read(ini);

    std::string& incKey = ini["Main"]["IncreaseKey"];
    std::string& decKey = ini["Main"]["DecreaseKey"];
    keyInc = std::stoi(incKey);
    keyDec = std::stoi(decKey);


    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&PointerKey, NULL, 0, NULL);
}

DWORD WINAPI PointerKey(LPVOID)
{
    while (true)
    {
        Sleep(100);
        GameState = *(int*)_TheGameFlowManager;
        if (GameState == 6)
        {
            if (GetAsyncKeyState(keyInc) & 1)
            {
                tod = tod + 0.1;
                if (tod >= 1)
                {
                    tod = 1;
                }
                SetTimeDay(tod);
                while ((GetAsyncKeyState(keyInc) & 0x8000) > 0) { Sleep(100); }
            }
            if (GetAsyncKeyState(keyDec) & 1)
            {
                tod = tod - 0.1;
                if (tod <= 0)
                {
                    tod = 0;
                }
                SetTimeDay(tod);
                while ((GetAsyncKeyState(keyDec) & 0x8000) > 0) { Sleep(100); }
            }
        }
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Main();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

