//
// Created by SyperOlao on 27.09.2025.
//

#ifndef HAMMURABI_CLONE_SAVEMANAGER_H
#define HAMMURABI_CLONE_SAVEMANAGER_H
#include "../core/GameState.h"
#include <string>
#include <optional>

class SaveManager {
public:
    static constexpr auto SAVE_FILE = "savegame.dat";

    static bool save(const GameState& state);
    static std::optional<GameState> load();
    static bool saveExists();
    static void remove_save();
};


#endif //HAMMURABI_CLONE_SAVEMANAGER_H