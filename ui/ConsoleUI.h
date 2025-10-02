//
// Created by SyperOlao on 27.09.2025.
//

#ifndef HAMMURABI_CLONE_CONSOLEUI_H
#define HAMMURABI_CLONE_CONSOLEUI_H
#include "../core/GameRepository.h"
#include "../core/GameState.h"
#include "validation/Validation.h"


class ConsoleUI {
public:
    explicit ConsoleUI(IGameRepository &repo);

    void set_last_input(const InputState &input);
    void set_last_harvest(int harvested_total, int yield_per_acre);


    void startup_message() const;


    static void death_message(GameState game_state);

    static void end_game(ResultGameStatistic result);

    [[nodiscard]] InputState input_message(InputState input_state) const;
    void show_round_summary_from_repo() const;

    void render(const GameState &s) const;

    static void hud(const GameState &s);
    ~ConsoleUI();

private:

    IGameRepository &repo_;
    IGameRepository::SubscriberToken token_;
    InputState last_input_;
    int last_harvested_total_ = 0;
    int last_yield_per_acre_ = 0;

    static void typewriter(const std::string &text, int delay_ms = 0);
    static void typewriter_cin(const std::string &text, int delay_ms = 1);

    static std::string prompt_until_valid(const std::string &field_key,
                                      const std::string &label,
                                      const Validation &validator,
                                      const GameState *state_ptr);

    static std::string trim(const std::string &s);
    static bool str_to_ll(const std::string &s, long long &out);

};


#endif //HAMMURABI_CLONE_CONSOLEUI_H
