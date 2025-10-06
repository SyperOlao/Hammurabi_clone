//
// Created by SyperOlao on 27.09.2025.
//

#include "ConsoleUI.h"

#include <format>
#include <iostream>
#include <sstream>
#include <thread>

#include "../core/Constants.h"
#include "../utils/Color.h"

ConsoleUI::ConsoleUI(IGameRepository &repo) : repo_(repo) {
    token_ = repo_.subscribe([this](const std::shared_ptr<const GameState> &snap) {
        render(*snap);
    });
}


void ConsoleUI::set_last_input(const InputState &input) {
    last_input_ = input;
}

void ConsoleUI::set_last_harvest(const int harvested_total, int yield_per_acre) {
    last_harvested_total_ = harvested_total;
    last_yield_per_acre_ = yield_per_acre;
}

void ConsoleUI::typewriter(const std::string &text, const int delay_ms) {
    for (const char c: text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    std::cout << std::endl;
}

void ConsoleUI::hud(const GameState &s) {
    std::ostringstream hud;
    hud << Color::NEON_YELLOW << "HUD: " << Color::RESET
            << "Популяция=" << Color::NEON_GREEN << s.population << Color::RESET << "  "
            << "Пшеница=" << Color::NEON_CYAN << s.wheat << Color::RESET << "  "
            << "Количество акров=" << Color::NEON_PURPLE << s.land << Color::RESET
            << "  " << Color::DIM << "(Стоимость акра=" << Color::NEON_BLUE << s.land_price << Color::RESET << ")" <<
            Color::RESET << "\n"
            << Color::NEON_YELLOW << "Каждый житель потребляет " << Color::NEON_GREEN << GameConsts::kConsumptionOfWheat
            << Color::RESET << Color::NEON_YELLOW << "бушелей пшеницы в год" << Color::RESET << std::endl
            << Color::NEON_YELLOW << "Каждый акр для засева требует " << Color::NEON_GREEN << static_cast<float>(1) / static_cast<float>(GameConsts::kWheatConsumptionForLand)
            << Color::RESET << Color::NEON_YELLOW <<  "бушелей пшеницы в год" << Color::RESET;

    typewriter(hud.str(), 0);
}

bool ConsoleUI::prompt_continue_saved_game() {
    typewriter(std::string(Color::NEON_YELLOW) +
               "Обнаружено сохранение предыдущей игры!" + Color::RESET, 0);
    typewriter(std::string(Color::NEON_CYAN) +
               "Хотите продолжить с того места, где остановились?" + Color::RESET, 0);

    while (true) {
        typewriter_cin(std::string(Color::NEON_PURPLE) + "» " + Color::RESET +
                       "Введите " + Color::NEON_GREEN + "y" + Color::RESET + " (да) или " +
                       Color::NEON_RED + "n/Enter" + Color::RESET + " (начать заново): ", 0);

        std::string input;
        if (!std::getline(std::cin, input)) {
            input = "n";
        }

        input = trim(input);
        if (input.empty()) {
            input = "n";
        }

        char c = std::tolower(static_cast<unsigned char>(input[0]));
        if (c == 'y') {
            std::cout << Color::NEON_GREEN << "✔ Продолжаем предыдущую игру." << Color::RESET << "\n\n";
            return true;
        }
        if (c == 'n') {
            std::cout << Color::NEON_YELLOW << "⚠ Начинаем новую игру. Старое сохранение будет удалено." << Color::RESET
                    << "\n\n";
            return false;
        }

        std::cout << Color::ERROR << "✖ Неверный ввод. Пожалуйста, введите 'y' или 'n'." << Color::RESET << "\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

bool ConsoleUI::prompt_save_and_exit(const GameState &current_state) {
    typewriter(std::string("\n") + Color::NEON_YELLOW +
               "⏸️  Начало нового года. Хотите сохранить прогресс и выйти из игры?" + Color::RESET, 0);

    while (true) {
        typewriter_cin(std::string(Color::NEON_PURPLE) + "» " + Color::RESET +
                       "Введите " + Color::NEON_GREEN + "y" + Color::RESET + " (сохранить и выйти) или " +
                       Color::NEON_CYAN + "n/Enter" + Color::RESET + " (продолжить игру): ", 0);

        std::string input;
        if (!std::getline(std::cin, input)) {
            input = "n";
        }

        input = trim(input);
        if (input.empty()) {
            input = "n";
        };

        char c = std::tolower(static_cast<unsigned char>(input[0]));
        if (c == 'y') {
            std::cout << Color::NEON_GREEN << "\n✔ Игра будет сохранена. До скорой встречи, повелитель!\n" <<
                    Color::RESET;
            return true;
        } else if (c == 'n') {
            std::cout << Color::NEON_CYAN << "\n▶ Продолжаем правление...\n" << Color::RESET;
            return false;
        }

        std::cout << Color::ERROR << "✖ Пожалуйста, введите 'y' или 'n'." << Color::RESET << "\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void ConsoleUI::typewriter_cin(const std::string &text, int delay_ms) {
    for (const char c: text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
}

std::string repeat_string(const std::string &s, const size_t n) {
    if (n == 0) return {};
    std::ostringstream os;
    for (size_t i = 0; i < n; ++i) os << s;
    return os.str();
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
               Color::RESET);
}

void ConsoleUI::death_message(GameState game_state) {
    const std::string message = std::format(
        "{}Вы проиграли, у вас умерло более {}% жителей за 1 год. Это позор, мой повелитель! Я вас изгоняю и займу ваше место!{}",
        Color::NEON_RED,
        GameConsts::kLooseDeathPopulation,
        Color::RESET
    );
    typewriter(message);
    subtitles();
}

std::string ConsoleUI::trim(const std::string &s) {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) --b;
    return s.substr(a, b - a);
}

bool ConsoleUI::str_to_ll(const std::string &s, long long &out) {
    const std::string t = trim(s);
    if (t.empty()) {
        out = 0;
        return true;
    }
    bool neg = false;
    size_t i = 0;
    if (t[0] == '+' || t[0] == '-') {
        neg = (t[0] == '-');
        i = 1;
    }
    if (i >= t.size()) return false;
    long long val = 0;
    for (; i < t.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(t[i]))) return false;
        const int d = t[i] - '0';
        if (val > (std::numeric_limits<long long>::max() - d) / 10) return false;
        val = val * 10 + d;
    }
    out = neg ? -val : val;
    return true;
}

std::string ConsoleUI::prompt_until_valid(const std::string &field_key, const std::string &label,
                                          const Validation &validator, const GameState *state_ptr) {
    std::string line;
    while (true) {
        std::ostringstream oss;
        oss << Color::NEON_CYAN << "» " << Color::RESET
                << Color::NEON_YELLOW << label << Color::RESET
                << " " << Color::DIM << "(введите число, Enter = 0)" << Color::RESET
                << "\n" << Color::NEON_PURPLE << ">>> " << Color::RESET;
        typewriter_cin(oss.str(), 0);


        if (!std::getline(std::cin, line)) {
            line = "0";
            std::cout << "\n";
        }
        line = trim(line);
        if (line.empty()) line = "0";

        auto [ok, message] = validator.validate_field(field_key, line, state_ptr);

        if (ok) {
            std::cout << Color::NEON_GREEN << "✔ " << Color::RESET
                    << Color::NEON_CYAN << label << Color::RESET
                    << " = " << Color::NEON_GREEN << line << Color::RESET << "\n\n";
            return line;
        }
        std::cout << Color::ERROR << "✖ Ошибка: " << Color::RESET
                << Color::NEON_RED << message << Color::RESET << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        std::cout << Color::DIM << "Попробуй ещё раз, но на этот раз внимательнее." << Color::RESET << "\n\n";
    }
}

void ConsoleUI::end_game(ResultGameStatistic result, GameMarkResults mark_results) {
    using namespace std;

    const int W = 78;
    auto center = [](const string &s, int w) {
        if ((int) s.size() >= w) return s;
        int pad = (w - (int) s.size()) / 2;
        return string(pad, ' ') + s;
    };

    {
        ostringstream h;
        h << "╔" << repeat_string("═", W - 2) << "╗\n";
        h << "║" << center(
            string(Color::NEON_PURPLE) + "После 10 раунда игра вычисляет следующие статистики:" + Color::RESET,
            W - 2) << "║\n";
        h << "╟" << repeat_string("═", W - 2) << "╢\n";
        typewriter(h.str(), 0);
    }

    // Значения P и L
    {
        int P = result.average_death_percent;
        int L = result.lend_for_person;

        ostringstream s;
        s << "  Среднегодовой процент умерших от голода "
                << Color::NEON_RED << "P = " << P << "%" << Color::RESET << "\n";
        s << "  Количество акров земли на одного жителя "
                << Color::NEON_CYAN << "L = " << L << Color::RESET << "\n";
        typewriter(s.str(), 0);
    }

    // Оценка по правилам
    string verdict_title;
    string verdict_text;

    switch (mark_results) {
        case Fail:
            verdict_title = string(Color::NEON_RED) + "Оценка: Плохо" + Color::RESET;
            verdict_text = "«Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города. "
                    "Теперь вы вынуждены влачить жалкое существование в изгнании»";
            break;
        case Satisfactorily:
            verdict_title = string(Color::NEON_YELLOW) + "Оценка: Удовлетворительно" + Color::RESET;
            verdict_text = "«Вы правили железной рукой, подобно Нерону и Ивану Грозному. Народ вздохнул с облегчением, "
                    "и никто больше не желает видеть вас правителем»";
            break;
        case Good:
            verdict_title = string(Color::NEON_CYAN) + "Оценка: Хорошо" + Color::RESET;
            verdict_text = "«Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, "
                    "но многие хотели бы увидеть вас во главе города снова»";
            break;
        case Excellent:
            verdict_title = string(Color::NEON_GREEN) + "Оценка: Отлично" + Color::RESET;
            verdict_text = "«Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше»";
            break;
    }
    {
        ostringstream v;
        v << "\n" << "╟" << repeat_string("═", W - 2) << "╢\n";
        v << "║ " << center(verdict_title, W - 4) << " ║\n";
        v << "╟" << repeat_string("═", W - 2) << "╢\n";

        constexpr int textWidth = W - 6;
        const string &t = verdict_text;
        size_t pos = 0;
        while (pos < t.size()) {
            size_t len = min(static_cast<size_t>(textWidth), t.size() - pos);
            if (pos + len < t.size()) {
                if (size_t space = t.rfind(' ', pos + len); space != string::npos && space > pos) {
                    len = space - pos;
                }
            }
            string chunk = t.substr(pos, len);
            while (!chunk.empty() && isspace(static_cast<unsigned char>(chunk.front()))) chunk.erase(chunk.begin());
            while (!chunk.empty() && isspace(static_cast<unsigned char>(chunk.back()))) chunk.pop_back();

            v << "║   " << center(chunk, textWidth) << "   ║\n";
            pos += len;
            while (pos < t.size() && isspace(static_cast<unsigned char>(t[pos]))) ++pos;
        }

        v << "╚" << repeat_string("═", W - 2) << "╝\n";
        typewriter(v.str(), 2);
    }

    {
        ostringstream fin;
        fin << "\n" << Color::NEON_YELLOW << "Итоги подсчёта:" << Color::RESET
                << " P=" << Color::NEON_RED << result.average_death_percent << "%" << Color::RESET
                << "  L=" << Color::NEON_CYAN << result.lend_for_person << Color::RESET << "\n";
        typewriter(fin.str());
    }
    subtitles();
}

InputState ConsoleUI::input_message(InputState input_state) const {
    const auto snap = repo_.get_snapshot();

    Validation validator;

    typewriter(std::string(Color::NEON_PURPLE) +
               "Время принимать решения, повелитель. Введите значения для хода." +
               Color::RESET + "\n", 0);

    auto temp_state = *snap;
    long long tmp = 0;
    hud(temp_state);
    std::string s_buy = prompt_until_valid("land_for_buy", "Купить акров земли", validator, &temp_state);
    str_to_ll(s_buy, tmp);
    long long cost = tmp * temp_state.land_price;
    temp_state.wheat -= static_cast<int>(cost);
    temp_state.land += static_cast<int>(tmp);
    hud(temp_state);
    std::string s_sell = prompt_until_valid("land_for_sell", "Продать акров земли", validator, &temp_state);
    str_to_ll(s_sell, tmp);
    temp_state.land -= static_cast<int>(tmp);
    temp_state.wheat += static_cast<int>(tmp * temp_state.land_price);
    hud(temp_state);
    std::string s_food = prompt_until_valid("wheat_for_food", "Зерно для еды", validator, &temp_state);
    str_to_ll(s_food, tmp);
    temp_state.wheat -= static_cast<int>(tmp);
    hud(temp_state);
    std::string s_sow = prompt_until_valid("wheat_for_sow", "Зерно для посева", validator, &temp_state);
    str_to_ll(s_sow, tmp);
    temp_state.wheat -= static_cast<int>(tmp);
    hud(temp_state);

    if (!str_to_ll(s_buy, tmp)) tmp = 0;
    input_state.land_for_buy = static_cast<int>(tmp);

    if (!str_to_ll(s_sell, tmp)) tmp = 0;
    input_state.land_for_sell = static_cast<int>(tmp);

    if (!str_to_ll(s_food, tmp)) tmp = 0;
    input_state.wheat_for_food = static_cast<int>(tmp);

    if (!str_to_ll(s_sow, tmp)) tmp = 0;
    input_state.wheat_for_sow = static_cast<int>(tmp);
    {
        std::ostringstream confirm;
        confirm << Color::NEON_YELLOW << "Ваш ход: " << Color::RESET
                << "Куплено земли=" << Color::NEON_GREEN << input_state.land_for_buy << Color::RESET << "  "
                << "Продано земли=" << Color::NEON_PURPLE << input_state.land_for_sell << Color::RESET << "  "
                << "Выдано пшена для еды=" << Color::NEON_CYAN << input_state.wheat_for_food << Color::RESET << "  "
                << "Выдано пшена для засева=" << Color::NEON_GREEN << input_state.wheat_for_sow << Color::RESET << "\n";
        typewriter(confirm.str(), 0);
    }

    return input_state;
}


void ConsoleUI::show_round_summary_from_repo() const {
    const auto snap = repo_.get_snapshot();
    const GameState *state = snap.get();

    constexpr int W = 76;

    auto center = [](const std::string &s, int w) {
        if (static_cast<int>(s.size()) >= w) return s;
        const int pad = (w - static_cast<int>(s.size())) / 2;
        return std::string(pad, ' ') + s;
    };

    {
        auto header_line = [&](const std::string &title) {
            std::ostringstream h;
            h << "╔" << repeat_string("═", W - 2) << "╗\n";
            h << "║" << center(title, W - 2) << "║\n";
            h << "╟" << repeat_string("─", W - 2) << "╢\n";
            return h.str();
        };
        std::string title = std::string(Color::NEON_PURPLE) + "Мой повелитель, доклад о годе " +
                            std::to_string(state->years) + Color::RESET;
        typewriter(header_line(title), 0);
    }

    if (state->death_from_starvation > 0) {
        std::ostringstream o;
        o << "  " << Color::NEON_RED << "☠ " << Color::RESET
                << Color::NEON_YELLOW << state->death_from_starvation << Color::RESET
                << " человек умерли от голода." << std::endl;
        typewriter(o.str(), 0);
    }
    if (state->plague) {
        std::ostringstream o;
        o << "  " << Color::NEON_RED << "☠ " << Color::RESET
                << Color::NEON_YELLOW << std::to_string(state->deaths) << Color::RESET
                << " всего человек умерло.";
        typewriter(o.str(), 0);
    }

    if (state->immigrants > 0) {
        std::ostringstream o;
        o << "  " << Color::NEON_GREEN << "⇪ " << Color::RESET
                << Color::NEON_YELLOW << state->immigrants << Color::RESET
                << " человек прибыли в наш великий город.";
        typewriter(o.str(), 0);
    }

    if (state->plague) {
        std::ostringstream o;
        o << "  " << Color::NEON_RED << "☣ Чума" << Color::RESET
                << Color::DIM << " — жестокая беда." << Color::RESET;
        typewriter(o.str(), 0);
    } else {
        typewriter(std::string("  ") + Color::NEON_CYAN + "✓ Чума не обнаружена." + Color::RESET, 0);
    }


    {
        std::ostringstream o;
        o << "  Население сейчас: " << Color::NEON_GREEN << state->population << Color::RESET << " человек.";
        typewriter(o.str(), 0);
    }
    {
        std::ostringstream o;
        o << "  Мы собрали " << Color::NEON_CYAN << state->sow_wheat_land << Color::RESET
                << " бушелей пшеницы, по " << Color::NEON_CYAN << state->harvest_yield << Color::RESET
                << " бушеля(ей) с акра.";
        typewriter(o.str(), 0);
    }


    if (state->destroyed_wheat > 0) {
        std::ostringstream o;
        o << "  Крысы истребили " << Color::NEON_RED << state->destroyed_wheat << Color::RESET
                << " бушелей, оставив " << Color::NEON_GREEN << state->wheat << Color::RESET << " в амбарах.";
        typewriter(o.str(), 0);
    } else {
        std::ostringstream o;
        o << "  Крыс не было. В амбарах " << Color::NEON_GREEN << state->wheat << Color::RESET << " бушелей.";
        typewriter(o.str(), 0);
    }

    {
        std::ostringstream o;
        o << "  Город сейчас занимает " << Color::NEON_PURPLE << state->land << Color::RESET << " акров.";
        typewriter(o.str(), 0);

        std::ostringstream o2;
        o2 << "  Стоимость акра в этом году: " << Color::NEON_YELLOW << state->land_price << Color::RESET <<
                " бушелей.";
        typewriter(o2.str(), 0);
    }

    typewriter("\n" + std::string("╟") + repeat_string("─", W - 2) + "╢\n", 0);

    {
        std::ostringstream o;
        o << "  " << Color::NEON_YELLOW << "Предыдущее решение повелителя:" << Color::RESET << "\n";
        o << "    Купить акров: " << Color::NEON_GREEN << last_input_.land_for_buy << Color::RESET << "  ";
        o << "Продать акров: " << Color::NEON_PURPLE << last_input_.land_for_sell << Color::RESET << "\n";
        o << "    Съесть бушелей: " << Color::NEON_CYAN << last_input_.wheat_for_food << Color::RESET << "  ";
        o << "Засеять бушелей: " << Color::NEON_GREEN << last_input_.wheat_for_sow << Color::RESET << "\n";
        typewriter(o.str(), 0);
    }

    typewriter(
        std::string("\n") + Color::NEON_YELLOW + "Что пожелаешь, повелитель?" + Color::RESET,
        0
    );

    std::ostringstream foot;
    foot << "\n" << "╚" << repeat_string("─", W - 2) << "╝\n";
    typewriter(foot.str(), 0);
}

void ConsoleUI::render(const GameState &s) const {
    // show_round_summary_from_repo();
}

void ConsoleUI::subtitles() {
    std::ostringstream fin;
    fin << std::endl;
    const auto line = "==================================";
    fin << Color::WHITE_BACK << line << Color::RESET << std::endl;
    fin << Color::WHITE_BACK << line << Color::RESET << std::endl;
    fin << Color::BLUE_WHITE_BACK << "Developed    by   " << " Anna Moklyakova" << Color::RESET;
    fin << "\n" << Color::BLUE_WHITE_BACK << "Sound and QA by" << " Konstantin Semenov" << Color::RESET << std::endl;
    fin << Color::RED_BACK << line << Color::RESET << std::endl;
    fin << Color::RED_BACK << line << Color::RESET << std::endl;
    typewriter(fin.str(), 0);
}


ConsoleUI::~ConsoleUI() {
    repo_.unsubscribe(token_);
}
