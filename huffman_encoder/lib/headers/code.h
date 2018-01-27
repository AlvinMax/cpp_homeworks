//
// Created by Maxim on 11.01.2018.
//

#ifndef CODE_H
#define CODE_H

#include "consts.h"
#include <vector>

using std::vector;

struct code
{
    code() = delete;
    code(vector<unsigned char> const &digits, size_t size) : digits(digits), size(size) {}

    
    vector <unsigned char> digits;
    size_t size;
};

#endif //CODE_H
