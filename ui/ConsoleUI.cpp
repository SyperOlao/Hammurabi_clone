//
// Created by SyperOlao on 27.09.2025.
//

#include "ConsoleUI.h"
#include <iostream>
#include <thread>

#include "../utils/Color.h"

ConsoleUI::ConsoleUI(IGameRepository &repo) : repo_(repo) {
    token_ = repo_.subscribe([this](const std::shared_ptr<const GameState> &snap) {
        render(*snap);
    });
}

void ConsoleUI::typewriter(const std::string &text, const int delay_ms) {
    for (const char c: text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    std::cout << std::endl;
}


void ConsoleUI::startup_message() const {
    const auto snap = repo_.get_snapshot();

    typewriter(std::string(Color::NEON_PURPLE) +
               "Добрый день мой повелитель, я ваш паж и сегодня ваш первый день правления!" +
               Color::RESET);

    typewriter(std::string(Color::NEON_CYAN) +
               "Хочу вас уведомить что наше государство Хаммураби сейчас находится на грани упадка:" +
               Color::RESET);

    typewriter(std::string(Color::NEON_YELLOW) +
               "А именно:" +
               Color::RESET);

    typewriter("В городе проживает " + std::string(Color::NEON_GREEN) +
               std::to_string(snap->population) + " человек" + Color::RESET);

    typewriter("В закромах города " + std::string(Color::NEON_CYAN) +
               std::to_string(snap->wheat) + " бушелей пшеницы" + Color::RESET);

    typewriter("Город занимает " + std::string(Color::NEON_PURPLE) +
               std::to_string(snap->land) + " акров земли" + Color::RESET);

    typewriter(std::string(Color::NEON_YELLOW) +
               "Очень надеюсь, что ваше правление приведет к процветанию нашей страны!" +
               Color::RESET, 50);
}

void ConsoleUI::render(const GameState &s) {
    std::cout << "Year " << s.years << " pop=" << s.population << " wheat=" << s.wheat << "\n";
}

ConsoleUI::~ConsoleUI() {
    repo_.unsubscribe(token_);
}
