//
// Created by SyperOlao on 27.09.2025.
//

#include "SaveManager.h"

#include <fstream>

bool SaveManager::save(const GameState &state) {
    std::ofstream file(SAVE_FILE);
    if (!file.is_open()) return false;

    file << state.population << '\n'
         << state.wheat << '\n'
         << state.harvest_yield << '\n'
         << state.destroyed_wheat << '\n'
         << state.land << '\n'
         << state.land_price << '\n'
         << state.years << '\n'
         << state.deaths << '\n'
         << state.immigrants << '\n'
         << state.plague << '\n';

    return file.good();
}

std::optional<GameState> SaveManager::load() {
    std::ifstream file(SAVE_FILE);
    if (!file.is_open()) return std::nullopt;

    GameState state;
    file >> state.population
         >> state.wheat
         >> state.harvest_yield
         >> state.destroyed_wheat
         >> state.land
         >> state.land_price
         >> state.years
         >> state.deaths
         >> state.immigrants
         >> state.plague;

    if (file.fail()) return std::nullopt;

    return state;
}

bool SaveManager::saveExists() {
    const std::ifstream file(SAVE_FILE);
    return file.good();
}

void SaveManager::remove_save() {
    std::remove(SAVE_FILE);
}
