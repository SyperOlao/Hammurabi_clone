//
// Created by SyperOlao on 02.10.2025.
//

#ifndef HAMMURABI_CLONE_GAMEREPOSITORY_H
#define HAMMURABI_CLONE_GAMEREPOSITORY_H
#include "GameState.h"
#include <functional>
#include <map>
#include <memory>
#include <mutex>

class IGameRepository {
public:
    using SubscriberToken = unsigned;
    using Callback = std::function<void(std::shared_ptr<const GameState>)>;

    virtual ~IGameRepository() = default;

    [[nodiscard]] virtual std::shared_ptr<const GameState> get_snapshot() const = 0;

    virtual void update_state(const std::function<void(GameState &)> &mutator) = 0;

    virtual SubscriberToken subscribe(Callback cb) = 0;

    virtual void unsubscribe(SubscriberToken token) = 0;
};


class GameRepository final : public IGameRepository {
public:
    GameRepository();

    std::shared_ptr<const GameState> get_snapshot() const override;

    void update_state(const std::function<void(GameState &)> &mutator) override;

    SubscriberToken subscribe(Callback cb) override;

    void unsubscribe(SubscriberToken token) override;

private:
    mutable std::mutex mutex_;
    std::shared_ptr<GameState> state_;
    std::map<SubscriberToken, Callback> subscribers_;
    SubscriberToken last_token_;
};


#endif //HAMMURABI_CLONE_GAMEREPOSITORY_H
