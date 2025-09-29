//
// Created by SyperOlao on 28.09.2025.
//

#ifndef HAMMURABI_CLONE_GAMELOGIC_H
#define HAMMURABI_CLONE_GAMELOGIC_H
#include "GameState.h"
#include <random>

class GameLogic {
    GameState game_state_;
    CurrentGameStatistic current_game_statistic_;
    InputState input_state_;
    std::mt19937 rng_;
    bool isLoose{false};

    [[nodiscard]] int wheat_consumption_for_population() const;

    int get_wheat_from_land();

    int wheat_after_loss_from_rats();

    void loose_condition_death_more_than_n_population();

    void prepare_game_state_before_next_round();

    void prepare_game_state_after_next_round();

    void feed_all_population();

    int max_process_land() const;

    int add_new_immigrates();

    void plague_disaster();

    [[nodiscard]] int wheat_consumption_for_land() const;

public:
    GameLogic();
    explicit GameLogic(const GameState &game_state, const InputState &input_state);
    auto next_round(const InputState &input_state) -> void;
    int get_current_price_for_land();
    CurrentGameStatistic end_game_results();
};


#endif //HAMMURABI_CLONE_GAMELOGIC_H
