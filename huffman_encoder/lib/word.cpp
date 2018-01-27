//
// Created by Maxim on 11.01.2018.
//

#include "word.h"

word::word() : code(0), len(0) {}
word::word(unsigned long long code, size_t len) : code(code), len(len) {}

void word::add(bool f) {
    code ^= (static_cast<unsigned long long> (f)) << len;
    ++len;
}

void word::erase() {
    --len;
}

bool word::get(size_t i) const {
    return static_cast<bool> (code & (1ULL << i));
}

void word::clear() {
    code = 0;
    len = 0;
}
