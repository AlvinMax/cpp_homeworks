//
// Created by Maxim on 11.01.2018.
//

#ifndef SUMMATOR_H
#define SUMMATOR_H

#include <vector>
#include "consts.h"
using std::vector;


struct summator
{
    summator();
    void add(vector<unsigned char> const &a);
    unsigned long long get(size_t i) const;
private:
    unsigned long long cnt[SYMB_RANGE + 1];
};

#endif //SUMMATOR_H
