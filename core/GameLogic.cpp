//
// Created by SyperOlao on 28.09.2025.
//

#include "GameLogic.h"
#include <algorithm>
#include "Constants.h"
#include <cassert>

GameLogic::GameLogic() : rng_(std::random_device{}()) {
};

GameLogic::GameLogic(const GameState &game_state, const InputState &input_state)
    : game_state_(game_state),
      input_state_(input_state),
      rng_(std::random_device{}()) {
}


int GameLogic::wheat_consumption_for_population() const {
    return current_game_statistic_.population * GameConsts::kConsumptionOfWheat;
}

void GameLogic::feed_all_population() {
    const int request_feed = std::max(0, input_state_.wheat_for_food);
    const int available_feed = std::max(0, game_state_.wheat);
    const int fed = std::min(request_feed, available_feed);
    game_state_.wheat -= fed;
    const int lack_wheat = wheat_consumption_for_population() - fed;

    int deaths = 0;

    if (lack_wheat > 0) {
        deaths = lack_wheat / GameConsts::kConsumptionOfWheat;
        if (deaths > current_game_statistic_.population) deaths = current_game_statistic_.population;
    }
    game_state_.deaths += deaths;
    current_game_statistic_.population -= deaths;
    if (current_game_statistic_.population < 0) game_state_.population = 0;
}


int GameLogic::get_current_price_for_land() {
    std::uniform_int_distribution price_dist(GameConsts::kMinPrice, GameConsts::kMaxPrice);
    game_state_.land_price = price_dist(rng_);
    return game_state_.land_price;
}

// TODO:: мб перенести в валидатор
int GameLogic::max_process_land() const {
    return current_game_statistic_.population * GameConsts::kLandMaxProcess;
}

int GameLogic::add_new_immigrates() {
    assert(GameConsts::kImmigrationDivider != 0);

    int immigrates = game_state_.deaths / 2 + (5 - game_state_.harvest_yield) * game_state_.wheat /
                     GameConsts::kImmigrationDivider + 1;
    immigrates = std::clamp(immigrates, 0, GameConsts::kMaxImmigrantAdd);
    game_state_.immigrants = immigrates;
    return immigrates;
}

int GameLogic::get_wheat_from_land() {
    std::uniform_int_distribution yield_dist(GameConsts::kWheatMinProcess,
                                             GameConsts::kWheatMaxProcess);
    const int yield_per_acre = yield_dist(rng_);
    game_state_.harvest_yield = yield_per_acre;
    game_state_.wheat += game_state_.land * yield_per_acre;
    return game_state_.wheat;
}

int GameLogic::wheat_after_loss_from_rats() {
    std::uniform_real_distribution<float> rat_dist(GameConsts::kRatsLossMin, GameConsts::kRatsLossMax);
    const auto wheat = static_cast<float>(game_state_.wheat);
    const auto destroyed_coefficient = rat_dist(rng_);

    const int wheat_after_loss = static_cast<int>(wheat * destroyed_coefficient);
    game_state_.destroyed_wheat = game_state_.wheat - wheat_after_loss;

    game_state_.wheat -= wheat_after_loss;
    if (game_state_.wheat < 0) game_state_.wheat = 0;
    return game_state_.wheat;
}

void GameLogic::loose_condition_death_more_than_n_population() {
    if (const int percent_death = (game_state_.deaths / game_state_.population) * 100; percent_death > GameConsts::kLooseDeathPopulation) {
        isLoose = true;
    }
}

void GameLogic::prepare_game_state_before_next_round() {
    game_state_.deaths = 0;
    game_state_.immigrants = 0;
    game_state_.plague = false;
    game_state_.destroyed_wheat = 0;
    game_state_.harvest_yield = 0;

    current_game_statistic_.population = game_state_.population;

}

void GameLogic::prepare_game_state_after_next_round() {
    input_state_ = InputState{};
    game_state_.population = current_game_statistic_.population;
    current_game_statistic_.average_death_percent += game_state_.deaths;
}

void GameLogic::plague_disaster() {
    constexpr int min_percent = 0;
    constexpr int max_percent = 100;
    if (std::uniform_int_distribution<int> plague_chance(min_percent, max_percent);
        plague_chance(rng_) <= GameConsts::kPlagueChanceMax) {
        current_game_statistic_.population *= (100 - GameConsts::kPlagueDeathPercent) / 100;
    }
}

int GameLogic::wheat_consumption_for_land() const {
    return game_state_.land / 2;
}

void GameLogic::next_round( const InputState &input_state) {
    prepare_game_state_before_next_round();
    input_state_ = input_state;
    game_state_.years++;

    get_wheat_from_land();
    wheat_after_loss_from_rats();

    feed_all_population();
    add_new_immigrates();
    plague_disaster();

    loose_condition_death_more_than_n_population();

    prepare_game_state_after_next_round();
}

CurrentGameStatistic GameLogic::end_game_results() {
    current_game_statistic_.average_death_percent /= game_state_.years;
    current_game_statistic_.lend_for_person = game_state_.population / game_state_.land;

    return current_game_statistic_;

}
