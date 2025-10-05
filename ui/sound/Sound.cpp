//
// Created by SyperOlao on 05.10.2025.
//

#include "Sound.h"
#include <fstream>
#include <iostream>
#include <windows.h>
#pragma comment(lib, "winmm.lib")

fs::path Sound::exe_dir() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    return fs::path(exePath).parent_path();
}

fs::path Sound::find_sound_upwards(const std::wstring &rel) {
    fs::path dir = exe_dir();
    for (int i = 0; i < 6; ++i) {
        fs::path cand = dir / rel;
        if (fs::exists(cand)) return cand;
        dir = dir.parent_path();
        if (dir.empty()) break;
    }
    return {};
}

void Sound::play_sound(const std::wstring &path_sound) {
    const auto sound = find_sound_upwards(path_sound);
    if (sound.empty()) {
        std::wcerr << L"Sound not found by walking up from exe dir. exe_dir=" << exe_dir() << L"\n";
        return;
    }
    PlaySoundW(sound.wstring().c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
}

double Sound::get_wav_duration(const std::wstring &path) {
    const auto sound_path = find_sound_upwards(path);
    std::ifstream file(sound_path, std::ios::binary);
    if (!file.is_open()) return 0.0;

    char riff[4];
    file.read(riff, 4);
    if (std::string(riff, 4) != "RIFF") return 0.0;

    file.seekg(4, std::ios::cur);

    char wave[4];
    file.read(wave, 4);
    if (std::string(wave, 4) != "WAVE") return 0.0;

    char fmt[4];
    file.read(fmt, 4);
    if (std::string(fmt, 4) != "fmt ") return 0.0;


    uint32_t fmtSize;
    file.read(reinterpret_cast<char*>(&fmtSize), 4);
    if (fmtSize < 16) return 0.0;

    uint16_t audioFormat;
    file.read(reinterpret_cast<char*>(&audioFormat), 2);
    if (audioFormat != 1) return 0.0; // только PCM

    uint16_t numChannels;
    file.read(reinterpret_cast<char*>(&numChannels), 2);

    uint32_t sampleRate;
    file.read(reinterpret_cast<char*>(&sampleRate), 4);

    file.seekg(6, std::ios::cur);

    uint16_t bitsPerSample;
    file.read(reinterpret_cast<char*>(&bitsPerSample), 2);


    char chunkHeader[4];
    uint32_t chunkSize;
    while (file.read(chunkHeader, 4)) {
        file.read(reinterpret_cast<char*>(&chunkSize), 4);
        if (std::string(chunkHeader, 4) == "data") {
            break;
        }
        file.seekg(chunkSize, std::ios::cur);
    }

    if (std::string(chunkHeader, 4) != "data") return 0.0;

    if (sampleRate == 0 || numChannels == 0 || bitsPerSample == 0) return 0.0;

    const double bytesPerSample = numChannels * (bitsPerSample / 8.0);
    const double duration = static_cast<double>(chunkSize) / (sampleRate * bytesPerSample);
    return duration;
}
