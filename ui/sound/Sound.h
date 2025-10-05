//
// Created by SyperOlao on 05.10.2025.
//

#ifndef HAMMURABI_CLONE_SOUND_H
#define HAMMURABI_CLONE_SOUND_H
#include <filesystem>

namespace fs = std::filesystem;

class Sound {
public:
    static void play_sound(const std::wstring &path_sound);
    static double get_wav_duration(const std::wstring& path);
private:
    static fs::path exe_dir();
    static fs::path find_sound_upwards(const std::wstring& rel);
};


#endif //HAMMURABI_CLONE_SOUND_H
