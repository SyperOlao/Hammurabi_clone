//
// Created by SyperOlao on 28.09.2025.
//

#include "GameLogic.h"
#include "Constants.h"


GameLogic::GameLogic() : rng_(std::random_device{}()) {
};

GameLogic::GameLogic(const GameState &game_state)
    : game_state_(game_state),
      rng_(std::random_device{}()) {
}

int GameLogic::wheat_consumption_for_human() const {
    return this->game_state_.population * 20;
}

int GameLogic::get_current_price_for_land() {
    std::uniform_int_distribution price_dist(GameConsts::kMinPrice, GameConsts::kMaxPrice);
    return price_dist(rng_);
}

int GameLogic::get_process_land() {
    std::uniform_int_distribution process_dist(GameConsts::kMinProcess, GameConsts::kMaxProcess);
    return process_dist(rng_);
}

int GameLogic::wheat_consumption_for_land() const {
    return static_cast<int>(std::round(this->game_state_.land * 0.5));
}

int GameLogic::next_round() {

}
