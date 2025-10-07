//
// Created by SyperOlao on 28.09.2025.
//

#include "../../ui/validation/Validation.h"

#include <format>

#include "../../utils/Utils.h"
#include "../../core/Constants.h"

Validation::Validation() {
    const long long KErrorNumber = 1000000000LL;
    register_rule("land_for_buy", [](const std::string &val, const GameState *state)-> ValidationResult {
        long long v;
        if (!Utils::parse_int64(val, v)) return ValidationResult::Bad("Incorrect input: not an integer");
        if (v < 0) return ValidationResult::Bad("Incorrect input value < 0");
        if (v == 0) return ValidationResult::Good();

        if (!state) return ValidationResult::Good();

        const long long cost = v * static_cast<long long>(state->land_price);
        if (cost > static_cast<long long>(state->wheat))
            return ValidationResult::Bad(
                "Incorrect input because you haven't enough wheat");

        if (v > KErrorNumber) return ValidationResult::Bad("Incorrect input: unrealistically large value");
        return ValidationResult::Good();
    });

    register_rule("land_for_sell", [](const std::string &val, const GameState *state)-> ValidationResult {
        long long v;
        if (!Utils::parse_int64(val, v)) return ValidationResult::Bad("Incorrect input: not an integer");
        if (v < 0) return ValidationResult::Bad("Incorrect input value < 0");
        if (v == 0) return ValidationResult::Good();

        if (!state) return ValidationResult::Good();

        if (v > static_cast<long long>(state->land))
            return ValidationResult::Bad(
                "Incorrect input because you haven't enough sell");

        if (v > KErrorNumber) return ValidationResult::Bad("Incorrect input: unrealistically large value");
        return ValidationResult::Good();
    });

    register_rule("wheat_for_food", [](const std::string &val, const GameState *state)-> ValidationResult {
        long long v;
        if (!Utils::parse_int64(val, v)) return ValidationResult::Bad("Incorrect input: not an integer");
        if (v < 0) return ValidationResult::Bad("Incorrect input value < 0");
        if (v == 0) return ValidationResult::Good();

        if (!state) return ValidationResult::Good();

        if (v > static_cast<long long>(state->wheat))
            return ValidationResult::Bad(
                "Incorrect input because you haven't enough wheat");

        if (v > KErrorNumber) return ValidationResult::Bad("Incorrect input: unrealistically large value");
        return ValidationResult::Good();
    });

    register_rule("wheat_for_sow", [](const std::string &val, const GameState *state)-> ValidationResult {
        long long v;
        if (!Utils::parse_int64(val, v)) return ValidationResult::Bad("Incorrect input: not an integer");
        if (v < 0) return ValidationResult::Bad("Incorrect input value < 0");
        if (v == 0) return ValidationResult::Good();

        if (!state) return ValidationResult::Good();

        if (v > static_cast<long long>(state->wheat))
            return ValidationResult::Bad(
                "Incorrect input because you haven't got enough wheat for sowing");
        if (v / GameConsts::kWheatConsumptionForLand > state->land)
            return ValidationResult::Bad(
                "Incorrect input because you haven't got enough land");
        if (v > KErrorNumber) return ValidationResult::Bad("Incorrect input: unrealistically large value");
        return ValidationResult::Good();
    });

    register_rule("buy_death_souls", [](const std::string &val, const GameState *state)-> ValidationResult {
        long long v;
        if (!Utils::parse_int64(val, v)) return ValidationResult::Bad("Incorrect input: not an integer");
        if (v < 0) return ValidationResult::Bad("Incorrect input value < 0");
        if (v == 0) return ValidationResult::Good();

        if (!state) return ValidationResult::Good();
        if (state->death_for_the_last_round == 0)
            return ValidationResult::Bad(
                "Nobody died");
        if (v > state->death_for_the_last_round * 100 /
            GameConsts::kPercentOfDeathToBuy)
            return ValidationResult::Bad(
                std::format(
                    "No one died in such quantities. Maximum {}% of mortality",
                    GameConsts::kPercentOfDeathToBuy));
        if (v > static_cast<long long>(state->death_souls_price * state->death_for_the_last_round * 100 /
                                       GameConsts::kPercentOfDeathToBuy))
            return ValidationResult::Bad(
                "Incorrect input because you haven't got enough wheat for buying souls");
        if (v > KErrorNumber) return ValidationResult::Bad("Incorrect input: unrealistically large value");
        return ValidationResult::Good();
    });
}


void Validation::register_rule(const std::string &key, ValidatorFn fn) {
    rules_[key] = std::move(fn);
}

ValidationResult Validation::validate_field(const std::string &key, const std::string &value,
                                            const GameState *state) const {
    const auto it = rules_.find(key);
    if (it == rules_.end()) {
        return ValidationResult::Bad("No validation rule for key: " + key);
    }
    return it->second(value, state);
}
