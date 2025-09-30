//
// Created by SyperOlao on 30.09.2025.
//
#pragma once

#include <string>

namespace Utils {
    static bool parse_int64(const std::string &s, long long &out) {
        if (s.empty()) return false;
        char *end = nullptr;
        errno = 0;
        const long long v = std::strtoll(s.c_str(), &end, 10);
        if (errno == ERANGE) return false;
        if (end == s.c_str() || *end != '\0') return false;
        out = v;
        return true;
    }
}
