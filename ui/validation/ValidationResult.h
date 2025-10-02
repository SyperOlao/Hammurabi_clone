//
// Created by SyperOlao on 30.09.2025.
//

#ifndef HAMMURABI_CLONE_VALIDATIONRESULT_H
#define HAMMURABI_CLONE_VALIDATIONRESULT_H
#include <string>
#include <functional>
#include "../../core/GameState.h"

struct ValidationResult {
    bool ok;
    std::string message;
    static ValidationResult Good() { return {true, ""}; }
    static ValidationResult Bad(const std::string &m) { return {false, m}; }

};

using ValidatorFn = std::function<ValidationResult(const std::string &value, const GameState *state)>;

#endif //HAMMURABI_CLONE_VALIDATIONRESULT_H
