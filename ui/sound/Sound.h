//
// Created by SyperOlao on 05.10.2025.
//

#ifndef HAMMURABI_CLONE_SOUND_H
#define HAMMURABI_CLONE_SOUND_H
#include <filesystem>

namespace fs = std::filesystem;

class Sound {
public:
    fs::path exe_dir();
    fs::path find_sound_upwards(const std::wstring& rel);
};


#endif //HAMMURABI_CLONE_SOUND_H
