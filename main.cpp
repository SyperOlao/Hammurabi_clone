#include <iostream>

#include "GameApp.h"
#include <windows.h>
#include <io.h>

#include <mmsystem.h>
#include <filesystem>
#pragma comment(lib, "winmm.lib")

namespace fs = std::filesystem;

fs::path exe_dir() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    return fs::path(exePath).parent_path();
}

fs::path find_sound_upwards(const std::wstring& rel) {
    fs::path dir = exe_dir();
    for (int i = 0; i < 6; ++i) {
        fs::path cand = dir / rel;
        if (fs::exists(cand)) return cand;
        dir = dir.parent_path();
        if (dir.empty()) break;
    }
    return {};
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    auto sound = find_sound_upwards(L"assets\\sound3.wav");
    if (sound.empty()) {
        std::wcerr << L"Sound not found by walking up from exe dir. exe_dir=" << exe_dir() << L"\n";
        return 1;
    }
    std::wcout << L"Using sound path: " << sound << L"\n";
    BOOL ok = PlaySoundW(sound.wstring().c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
    std::wcout << (ok ? L"Play sound OK\n" : L"Play FAIL\n");



    GameApp engine;
    engine.start_game();
    engine.game_loop();
    return 0;
}
