//
// Created by SyperOlao on 27.09.2025.
//

#ifndef HAMMURABI_CLONE_GAMEAPP_H
#define HAMMURABI_CLONE_GAMEAPP_H
#include "core/GameLogic.h"
#include "core/GameRepository.h"
#include "persistence/SaveManager.h"
#include "ui/ConsoleUI.h"


class GameApp {
public:
    GameApp();
    void start_game();
    void game_loop();
    void end_game();

private:
    GameRepository repo_;
    GameLogic logic_;
    ConsoleUI ui_;
    SaveManager save_;
    bool running_ = false;
    const int kMaxYears = 10;

    bool try_load_saved_game();
    bool handle_save_and_exit_if_requested();
};


#endif //HAMMURABI_CLONE_GAMEAPP_H