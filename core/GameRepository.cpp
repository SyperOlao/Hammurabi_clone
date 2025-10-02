//
// Created by SyperOlao on 02.10.2025.
//

#include "GameRepository.h"

#include <ranges>
#include <utility>

GameRepository::GameRepository()
        : state_(std::make_shared<GameState>()),
          last_token_(0)
{}


std::shared_ptr<const GameState> GameRepository::get_snapshot() const {
    std::lock_guard<std::mutex> lg(mutex_);
    return state_;
}

void GameRepository::update_state(const std::function<void(GameState &)> &mutator) {
    std::map<SubscriberToken, Callback> subs_copy;
    std::shared_ptr<GameState> new_snapshot;
    {
        std::lock_guard<std::mutex> lg(mutex_);
        mutator(*state_);
        new_snapshot = state_;
        subs_copy = subscribers_;
    }

    for (auto &val: subs_copy | std::views::values) {
        try {
            val(new_snapshot);
        } catch (...) {
        }
    }
}

IGameRepository::SubscriberToken GameRepository::subscribe(Callback cb) {
    std::shared_ptr<const GameState> snapshot;
    SubscriberToken token;
    {
        std::lock_guard<std::mutex> lg(mutex_);
        token = ++last_token_;
        subscribers_.emplace(token, std::move(cb));
        snapshot = state_;
    }
    subscribers_.at(token)(snapshot);
    return token;
}

void GameRepository::unsubscribe(const SubscriberToken token) {
    std::lock_guard<std::mutex> lg(mutex_);
    subscribers_.erase(token);
}
