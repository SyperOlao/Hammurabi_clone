//
// Created by SyperOlao on 28.09.2025.
//

#ifndef HAMMURABI_CLONE_GAMELOGIC_H
#define HAMMURABI_CLONE_GAMELOGIC_H
#include "GameState.h"
#include <random>

class GameLogic {
    GameState game_state_;
    ResultGameStatistic result_game_statistic_;
    InputState input_state_;
    std::mt19937 rng_;
    bool isLose{false};
    int starting_population_{0};


    int get_wheat_from_land();

    int wheat_after_loss_from_rats();

    void check_loss_condition_by_death_percentage();

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
    void next_round(const InputState &input_state);
    int get_current_price_for_land();
    ResultGameStatistic end_game_results();
};


#endif //HAMMURABI_CLONE_GAMELOGIC_H
