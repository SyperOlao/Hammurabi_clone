#include <iostream>

#include "GameApp.h"
#include <windows.h>
#include <io.h>

#include <mmsystem.h>
#include <filesystem>

#pragma comment(lib, "winmm.lib")

void app_settings();

int main() {
    app_settings();


    GameApp engine;
    engine.start_game();
    engine.game_loop();
    return 0;
}


void app_settings() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
