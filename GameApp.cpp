//
// Created by SyperOlao on 27.09.2025.
//

#include "GameApp.h"

#include <iostream>
#include <thread>

#include "core/Constants.h"
#include "utils/Color.h"

GameApp::GameApp() : repo_(), logic_(repo_), ui_(repo_), sound_() {

}

void GameApp::start_game() {
    Sound::play_sound(GameSounds::kStartGame);
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
            Sound::play_sound(GameSounds::kGameLoose);
            std::this_thread::sleep_for(std::chrono::duration<double>(Sound::get_wav_duration(GameSounds::kGameLoose)));
            return;
        }

        if (state.years >= kMaxYears) {
            end_game();
            return;
        }

        Sound::play_sound(GameSounds::kNextRound);


        logic_.get_current_price_for_land();
        InputState input = ui_.input_message({});

        ui_.set_last_input(input);

        logic_.next_round(input);
        if (state.years >= 1) {
            ui_.show_round_summary_from_repo();
            if (state.plague) {
                Sound::play_sound(GameSounds::kPlague);
            }else {
                Sound::play_sound(GameSounds::kNoPlague);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }
}

void GameApp::end_game() {
    running_ = false;
    const auto mark_results = logic_.get_result_mark();
    SaveManager::remove_save();
    switch (mark_results) {
        case Fail:
            Sound::play_sound(GameSounds::kGameLoose);
            break;
        case Satisfactorily:
        case Good:
        case Excellent:
            Sound::play_sound(GameSounds::kGameWin);
            break;
    }
    ConsoleUI::end_game(logic_.calculate_end_game_results(), mark_results);
    std::this_thread::sleep_for(std::chrono::duration<double>(Sound::get_wav_duration(GameSounds::kGameLoose)));
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
