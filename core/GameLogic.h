//
// Created by SyperOlao on 28.09.2025.
//

#ifndef HAMMURABI_CLONE_GAMELOGIC_H
#define HAMMURABI_CLONE_GAMELOGIC_H
#include "GameState.h"
#include <random>

class GameLogic {
private:
    GameState game_state_;
    GameState current_game_state_;
    std::mt19937 rng_;
public:
    GameLogic();

    explicit GameLogic(const GameState &game_state);

    int wheat_consumption_for_human() const;

    int get_current_price_for_land();

    int get_process_land();

    int wheat_consumption_for_land() const;

    int next_round();
};


#endif //HAMMURABI_CLONE_GAMELOGIC_H
