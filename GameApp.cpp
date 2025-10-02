//
// Created by SyperOlao on 27.09.2025.
//

#include "GameApp.h"

#include <iostream>
#include <thread>

#include "utils/Color.h"

GameApp::GameApp() : repo_(), logic_(repo_), ui_(repo_) {

}

void GameApp::start_game() {
    try_load_saved_game();
    running_ = true;
    ui_.startup_message();
}

void GameApp::game_loop() {
    while (running_) {
        if (const bool run = handle_save_and_exit_if_requested(); !run) {
            break;
        }
        const auto snap = repo_.get_snapshot();
        const GameState &state = *snap;


        if (logic_.is_game_over()) {
            ui_.show_round_summary_from_repo();
            ConsoleUI::death_message(state);
            running_ = false;
            SaveManager::remove_save();
            break;
        }

        if (state.years >= kMaxYears) {
            running_ = false;
            ConsoleUI::end_game(logic_.end_game_results());
            SaveManager::remove_save();
            break;
        }
        logic_.get_current_price_for_land();
        InputState input = ui_.input_message({});

        ui_.set_last_input(input);

        logic_.next_round(input);
        if (state.years >= 1) {
            ui_.show_round_summary_from_repo();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }
}

void GameApp::end_game() {
    const ResultGameStatistic stats = logic_.end_game_results();
    ConsoleUI::end_game(stats);
}

bool GameApp::try_load_saved_game() {
    if (!SaveManager::saveExists()) {
        return false;
    }

    if (ConsoleUI::prompt_continue_saved_game()) {
        if (auto saved = SaveManager::load()) {
            repo_.update_state([&saved](GameState& state) {
                state = *saved;
            });
            return true;
        }
        std::cout << Color::ERROR << "❌ Ошибка загрузки сохранения. Начинаем новую игру." << Color::RESET << "\n";
    }

    std::remove(SaveManager::SAVE_FILE);
    return false;
}

bool GameApp::handle_save_and_exit_if_requested() {
    bool result = true;
    if (const auto snap = repo_.get_snapshot(); ConsoleUI::prompt_save_and_exit(*snap)) {
        if (SaveManager::save(*snap)) {
            running_ = false;
            result = false;
        } else {
            std::cout << Color::ERROR << "❌ Не удалось сохранить игру!" << Color::RESET << "\n";
            running_ = false;
            result = false;
        }
    }
    return result;
}
