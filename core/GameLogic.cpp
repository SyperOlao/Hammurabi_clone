//
// Created by SyperOlao on 28.09.2025.
//

#include "GameLogic.h"
#include "Constants.h"


GameLogic::GameLogic() : rng_(std::random_device{}()) {
};

GameLogic::GameLogic(const GameState &game_state, const InputState &input_state)
    : game_state_(game_state),
      input_state_(input_state),
      rng_(std::random_device{}()) {
}


int GameLogic::wheat_consumption_for_population() const {
    return game_state_.population * GameConsts::kConsumptionOfWheat;
}

void GameLogic::feed_all_population() {
    const int request_feed = std::max(0, input_state_.wheat_for_food);
    const int available_feed = std::max(0, game_state_.wheat);
    const int fed = std::min(request_feed, available_feed);
    game_state_.wheat -= fed;
    const int lack_wheat = wheat_consumption_for_population() - fed;

    int deaths = 0;

    if (lack_wheat > 0) {
        deaths =lack_wheat / GameConsts::kConsumptionOfWheat;
        if (deaths > game_state_.population) deaths = game_state_.population;
    }
    game_state_.deaths += deaths;
    game_state_.population -= deaths;
    if (game_state_.population < 0) game_state_.population = 0;
}


int GameLogic::get_current_price_for_land() {
    std::uniform_int_distribution price_dist(GameConsts::kMinPrice, GameConsts::kMaxPrice);
    return price_dist(rng_);
}

// TODO:: мб перенести в валидатор
int GameLogic::max_process_land() const {
    return game_state_.population * GameConsts::kLandMaxProcess;
}

int GameLogic::add_new_immigrates() {

}

int GameLogic::get_wheat_from_land() {
    std::uniform_int_distribution wheat_dist(GameConsts::kWheatMinProcess, GameConsts::kWheatMaxProcess);
    int result = 0;
    for (int i = 0; i < game_state_.land; ++i) {
        result += wheat_dist(rng_);
    }
    return result;
}

int GameLogic::wheat_after_loss_from_rats() {
    std::uniform_real_distribution<float> rat_dist(GameConsts::kRatsLossMin, GameConsts::kRatsLossMax);
    const auto sum_wheat =  static_cast<float>(game_state_.wheat + get_wheat_from_land());
    return static_cast<int>(sum_wheat * rat_dist(rng_));
}

void GameLogic::loose_condition_death_more_than_n_population() {

}

void GameLogic::plague_disaster() {

}

int GameLogic::wheat_consumption_for_land() const {
    return game_state_.land / 2;
}

int GameLogic::next_round() {
    game_state_.years++;
}
