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

void GameLogic::feed_all_population() {
    const int request_feed = std::max(0, input_state_.wheat_for_food);
    const int available_feed = std::max(0, game_state_.wheat);
    const int fed = std::min(request_feed, available_feed);

    game_state_.wheat -= fed;

    const int needed = starting_population_ * GameConsts::kConsumptionOfWheat;

    int deaths = 0;
    if (fed < needed) {
        const int lack = needed - fed;
        deaths = lack / GameConsts::kConsumptionOfWheat;
        if (deaths > game_state_.population) deaths = game_state_.population;
    }
    game_state_.deaths = deaths;
    game_state_.population -= deaths;
    if (game_state_.population < 0) game_state_.population = 0;
}


int GameLogic::get_current_price_for_land() {
    std::uniform_int_distribution price_dist(GameConsts::kMinPrice, GameConsts::kMaxPrice);
    game_state_.land_price = price_dist(rng_);
    return game_state_.land_price;
}

// TODO:: мб перенести в валидатор
int GameLogic::max_process_land() const {
    return game_state_.population * GameConsts::kLandMaxProcess;
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

    const long long added = static_cast<long long>(game_state_.land) * yield_per_acre;
    game_state_.wheat = static_cast<int>(std::min<long long>(std::numeric_limits<int>::max(),
                                                             static_cast<long long>(game_state_.wheat) + added));
    return game_state_.wheat;
}

int GameLogic::wheat_after_loss_from_rats() {
    std::uniform_real_distribution<float> rat_dist(GameConsts::kRatsLossMin, GameConsts::kRatsLossMax);
    const auto wheat = static_cast<float>(game_state_.wheat);
    const auto destroyed_fraction = rat_dist(rng_);

    const int destroyed = static_cast<int>(wheat * destroyed_fraction);
    const int remaining = wheat - destroyed;
    game_state_.destroyed_wheat = destroyed;

    game_state_.wheat = std::max(0, remaining);
    return game_state_.wheat;
}

void GameLogic::check_loss_condition_by_death_percentage() {
    if (starting_population_ <= 0) return;
    const int population_before = game_state_.population + game_state_.deaths - game_state_.immigrants;
    if (population_before <= 0) return;
    if (const int percent_death = static_cast<int>(
            static_cast<long long>(game_state_.deaths) * 100 / population_before);
        percent_death > GameConsts::kLooseDeathPopulation) {
        isLose = true;
    }
}

void GameLogic::prepare_game_state_before_next_round() {
    game_state_.deaths = 0;
    game_state_.immigrants = 0;
    game_state_.plague = false;
    game_state_.destroyed_wheat = 0;
    game_state_.harvest_yield = 0;
}

void GameLogic::prepare_game_state_after_next_round() {
    if (starting_population_ > 0) {
        const int percent_death_this_year = static_cast<int>(
            (static_cast<long long>(game_state_.deaths) * 100) / starting_population_);
        result_game_statistic_.average_death_percent += percent_death_this_year;
    }
    game_state_.population += game_state_.immigrants;
    input_state_ = InputState{};
}

void GameLogic::plague_disaster() {
    constexpr int min_percent = 0;
    constexpr int max_percent = 100;
    if (std::uniform_int_distribution<int> plague_chance(min_percent, max_percent);
        plague_chance(rng_) <= GameConsts::kPlagueChanceMax) {
        game_state_.plague = true;
        const int before = game_state_.population;
        const int after = static_cast<int>(static_cast<long long>(before) * (100 - GameConsts::kPlagueDeathPercent) /
                                           100);
        game_state_.deaths += before - after;
        game_state_.population = after;
    }
}

int GameLogic::wheat_consumption_for_land() const {
    return game_state_.land / 2;
}

void GameLogic::next_round(const InputState &input_state) {
    starting_population_ = game_state_.population;
    prepare_game_state_before_next_round();
    input_state_ = input_state;
    game_state_.years++;

    get_current_price_for_land();

    get_wheat_from_land();

    wheat_after_loss_from_rats();

    feed_all_population();
    add_new_immigrates();
    plague_disaster();

    check_loss_condition_by_death_percentage();

    prepare_game_state_after_next_round();
}

ResultGameStatistic GameLogic::end_game_results() {
    if (game_state_.population > 0) {
        result_game_statistic_.lend_for_person = game_state_.land / game_state_.population;
    } else {
        result_game_statistic_.lend_for_person = 0;
    }
    if (game_state_.years > 0) {
        result_game_statistic_.average_death_percent =
                result_game_statistic_.average_death_percent / game_state_.years;
    } else {
        result_game_statistic_.average_death_percent = 0;
    }
    return result_game_statistic_;
}
