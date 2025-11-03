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
         << state.sow_wheat_land << '\n'
         << state.land << '\n'
         << state.land_price << '\n'
         << state.death_souls_price << '\n'
         << state.years << '\n'
         << state.deaths << '\n'
         << state.death_from_starvation << '\n'
         << state.immigrants << '\n'
         << state.death_souls << '\n'
         << state.death_for_the_last_round << '\n'
         << state.plague << '\n'
         << state.inspector << '\n';

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
         >> state.sow_wheat_land
         >> state.land
         >> state.land_price
         >> state.death_souls_price
         >> state.years
         >> state.deaths
         >> state.death_from_starvation
         >> state.immigrants
         >> state.death_souls
         >> state.death_for_the_last_round
         >> state.plague
         >> state.inspector;

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
