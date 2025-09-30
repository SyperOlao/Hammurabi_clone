//
// Created by SyperOlao on 28.09.2025.
//

#ifndef HAMMURABI_CLONE_VALIDATION_H
#define HAMMURABI_CLONE_VALIDATION_H
#include <string>
#include <unordered_map>
#include "../../ui/validation/ValidationResult.h"

class Validation {
    std::unordered_map<std::string, ValidatorFn> rules_;
public:
    Validation();
    void register_rule(const std::string &key, ValidatorFn fn);
    ValidationResult validate_field(const std::string &key, const std::string &value, const GameState *state = nullptr) const;
};


#endif //HAMMURABI_CLONE_VALIDATION_H
