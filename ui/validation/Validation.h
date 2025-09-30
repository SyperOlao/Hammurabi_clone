//
// Created by SyperOlao on 28.09.2025.
//

#ifndef HAMMURABI_CLONE_VALIDATION_H
#define HAMMURABI_CLONE_VALIDATION_H
#include <string>
#include "../../ui/validation/ValidationResult.h"

class Validation {
public:
    Validation();
    void register_rule(const std::string &key, ValidatorFn fn);
    ValidationResult validate_field(const std::string &key, const std::string &value, const GameState *state = nullptr) const;
};


#endif //HAMMURABI_CLONE_VALIDATION_H
