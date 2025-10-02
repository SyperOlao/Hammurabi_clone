//
// Created by SyperOlao on 27.09.2025.
//

#include "GameApp.h"

#include <iostream>
#include <thread>

GameApp::GameApp() : repo_(), logic_(repo_), ui_(repo_) {

}

void GameApp::start_game() {
    running_ = true;
    ui_.startup_message();
}

void GameApp::game_loop() {
    while (running_) {
        const auto snap = repo_.get_snapshot();
        const GameState &state = *snap;


        if (logic_.is_game_over()) {
            ui_.show_round_summary_from_repo();
            ConsoleUI::death_message(state);
            running_ = false;
            break;
        }

        if (state.years >= kMaxYears) {
            running_ = false;
            ConsoleUI::end_game(logic_.end_game_results());
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
