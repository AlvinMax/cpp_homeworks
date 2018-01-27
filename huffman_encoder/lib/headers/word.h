//
// Created by Maxim on 11.01.2018.
//

#ifndef WORD_H
#define WORD_H

#include "consts.h"

struct word
{
    word();
    word(unsigned long long code, size_t len);

    void add(bool f);
    void erase();
    bool get(size_t i) const;
    void clear();

    unsigned long long code;
    size_t len;
};

#endif //WORD_H
