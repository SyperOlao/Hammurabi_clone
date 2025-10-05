//
// Created by SyperOlao on 28.09.2025.
//

#include "GameLogic.h"
#include <algorithm>
#include "Constants.h"
#include <cassert>


GameLogic::GameLogic(IGameRepository &repo)
    : repo_(repo), rng_(std::random_device{}()) {
}

void GameLogic::feed_all_population(GameState &s) const {
    const int request_feed = std::max(0, input_state_.wheat_for_food);
    const int available_feed = std::max(0, s.wheat);
    const int fed = std::min(request_feed, available_feed);

    s.wheat -= fed;

    const int needed = starting_population_ * GameConsts::kConsumptionOfWheat;

    int deaths = 0;
    if (fed < needed) {
        const int lack = needed - fed;
        deaths = lack / GameConsts::kConsumptionOfWheat;
        if (deaths > s.population) deaths = s.population;
    }
    s.deaths = deaths;
    s.population -= deaths;
    if (s.population < 0) s.population = 0;
}


int GameLogic::get_current_price_for_land() {
    std::uniform_int_distribution<int> price_dist(GameConsts::kMinPrice, GameConsts::kMaxPrice);
    const int new_price = price_dist(rng_);
    repo_.update_state([new_price](GameState &s) {
        s.land_price = new_price;
    });
    return new_price;
}

int GameLogic::max_process_land(const GameState &s) {
    return s.population * GameConsts::kLandMaxProcess;
}

int GameLogic::add_new_immigrates(GameState &s) {
    assert(GameConsts::kImmigrationDivider != 0);

    int immigrates = s.deaths / 2 + (5 - s.harvest_yield) * s.wheat /
                     GameConsts::kImmigrationDivider + 1;
    immigrates = std::clamp(immigrates, 0, GameConsts::kMaxImmigrantAdd);
    s.immigrants = immigrates;
    return immigrates;
}

int GameLogic::get_wheat_from_land(GameState &s) {
    std::uniform_int_distribution yield_dist(GameConsts::kWheatMinProcess,
                                             GameConsts::kWheatMaxProcess);
    const int yield_per_acre = yield_dist(rng_);
    const int max_sow_population = s.population * GameConsts::kLandMaxProcess;
    const int max_sow_wheat = input_state_.wheat_for_sow * GameConsts::kWheatConsumptionForLand;
    int min_value = std::min(max_sow_population, max_sow_wheat);
    if (min_value > s.land) {
        min_value = s.land;
    }
    s.harvest_yield = yield_per_acre;

    const long long added = static_cast<long long>(min_value) * yield_per_acre;
    s.sow_wheat_land = added;
    int sow = input_state_.wheat_for_sow;
    if (sow > s.wheat) {
        sow = s.wheat;
    }
    s.wheat -= sow;
    s.wheat = static_cast<int>(std::min<long long>(std::numeric_limits<int>::max(),
                                                   static_cast<long long>(s.wheat) + added));
    return s.wheat;
}

int GameLogic::wheat_after_loss_from_rats(GameState &s) {
    std::uniform_real_distribution<float> rat_dist(GameConsts::kRatsLossMin, GameConsts::kRatsLossMax);
    const auto wheat = static_cast<float>(s.wheat);
    const auto destroyed_fraction = rat_dist(rng_);

    const int destroyed = static_cast<int>(wheat * destroyed_fraction);
    const int remaining = static_cast<int>(wheat) - destroyed;
    s.destroyed_wheat = destroyed;

    s.wheat = std::max(0, remaining);
    return s.wheat;
}

void GameLogic::check_loss_condition_by_death_percentage(const GameState &s) {
    if (starting_population_ <= 0) return;
    const int percent_death = static_cast<int>(static_cast<long long>(s.deaths) * 100 / starting_population_);
    if (percent_death > GameConsts::kLooseDeathPopulation) {
        isLose = true;
    }
}

void GameLogic::prepare_game_state_before_next_round(GameState &s) {
    s.deaths = 0;
    s.immigrants = 0;
    s.plague = false;
    s.destroyed_wheat = 0;
    s.harvest_yield = 0;
    s.sow_wheat_land = 0;
}

void GameLogic::prepare_game_state_after_next_round(GameState &s) {
    if (starting_population_ > 0) {
        const int percent_death_this_year = static_cast<int>(
            (static_cast<long long>(s.deaths) * 100) / starting_population_);
        result_game_statistic_.average_death_percent += percent_death_this_year;
    }
    s.population += s.immigrants;
    input_state_ = InputState{};
}

void GameLogic::plague_disaster(GameState &s) {
    constexpr int min_percent = 0;
    constexpr int max_percent = 100;
    if (std::uniform_int_distribution<int> plague_chance(min_percent, max_percent);
        plague_chance(rng_) <= GameConsts::kPlagueChanceMax) {
        s.plague = true;
        const int before = s.population;
        const int after = static_cast<int>(static_cast<long long>(before) * (100 - GameConsts::kPlagueDeathPercent) /
                                           100);
        s.deaths += before - after;
        s.population = after;
    }
}

void GameLogic::sell_land(GameState &s) const {
    int selling_land = input_state_.land_for_sell;
    if (selling_land < 0) return;
    if (selling_land > s.land) {
        selling_land = s.land;
    }

    s.wheat += selling_land * s.land_price;
    s.land -= selling_land;
}

void GameLogic::buy_land(GameState &s) const {
    int buying_land = input_state_.land_for_buy;
    if (buying_land < 0) return;
    if (const int max_buying_land = s.wheat * s.land_price; buying_land > max_buying_land) {
        buying_land = max_buying_land;
    }
    s.wheat -= buying_land;
    s.land += buying_land / s.land_price;
}

int GameLogic::wheat_consumption_for_land(const GameState &s) {
    return s.land / GameConsts::kWheatConsumptionForLand;
}


void GameLogic::next_round(const InputState &input_state) {
    input_state_ = input_state;

    repo_.update_state([this,&input_state](GameState &s) {
        starting_population_ = s.population;
        prepare_game_state_before_next_round(s);

        s.years++;
        buy_land(s);
        sell_land(s);
        // get_current_price_for_land(s);

        get_wheat_from_land(s);

        wheat_after_loss_from_rats(s);

        feed_all_population(s);
        add_new_immigrates(s);
        plague_disaster(s);
        check_loss_condition_by_death_percentage(s);

        prepare_game_state_after_next_round(s);
    });
}

ResultGameStatistic GameLogic::end_game_results() {
    const auto snap = repo_.get_snapshot();
    if (!snap) return result_game_statistic_;
    if (snap->population > 0) {
        result_game_statistic_.lend_for_person = snap->land / snap->population;
    } else {
        result_game_statistic_.lend_for_person = 0;
    }
    if (snap->years > 0) {
        result_game_statistic_.average_death_percent =
                result_game_statistic_.average_death_percent / snap->years;
    } else {
        result_game_statistic_.average_death_percent = 0;
    }
    return result_game_statistic_;
}

bool GameLogic::is_game_over() const {
    return isLose;
}
