//
// Created by SyperOlao on 28.09.2025.
//

#ifndef HAMMURABI_CLONE_GAMELOGIC_H
#define HAMMURABI_CLONE_GAMELOGIC_H
#include "GameState.h"
#include <random>

#include "GameRepository.h"

class GameLogic {
public:
    explicit GameLogic(IGameRepository &repo);

    void next_round(const InputState &input_state);

    int get_current_price_for_land(GameState &s);

    ResultGameStatistic end_game_results();
    [[nodiscard]] bool is_game_over() const;

private:
    IGameRepository &repo_;
    ResultGameStatistic result_game_statistic_;
    InputState input_state_;
    std::mt19937 rng_;
    bool isLose{false};
    int starting_population_{0};


    int get_wheat_from_land(GameState &s);

    int wheat_after_loss_from_rats(GameState &s);

    void check_loss_condition_by_death_percentage(const GameState &s);

    static void prepare_game_state_before_next_round(GameState &s);

    void prepare_game_state_after_next_round(GameState &s);

    void feed_all_population(GameState &s) const;

    [[nodiscard]] static int max_process_land(const GameState &s);

    static int add_new_immigrates(GameState &s);

    void plague_disaster(GameState &s);

    [[nodiscard]] static int wheat_consumption_for_land(const GameState &s);
};


#endif //HAMMURABI_CLONE_GAMELOGIC_H
