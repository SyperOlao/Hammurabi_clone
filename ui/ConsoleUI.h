//
// Created by SyperOlao on 27.09.2025.
//

#ifndef HAMMURABI_CLONE_CONSOLEUI_H
#define HAMMURABI_CLONE_CONSOLEUI_H
#include "../core/GameRepository.h"
#include "../core/GameState.h"


class ConsoleUI {
public:
    ConsoleUI(IGameRepository &repo);

    void startup_message() const;

    void new_round(GameState game_state);

    void death_message(GameState game_state);

    void end_game(GameState game_state);

    void input_message(GameState game_state, InputState input_state);

    void render(const GameState &s);

    ~ConsoleUI();

private:
    IGameRepository &repo_;
    IGameRepository::SubscriberToken token_;

    static void typewriter(const std::string &text, int delay_ms = 40);
};




#endif //HAMMURABI_CLONE_CONSOLEUI_H
