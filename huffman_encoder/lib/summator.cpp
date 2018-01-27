//
// Created by Maxim on 11.01.2018.
//

#include <cstring>
#include "summator.h"

summator::summator()
{
    memset(cnt, 0, (SYMB_RANGE + 1) * sizeof(unsigned long long));
}

void summator::add(vector <unsigned char> const &a)
{
    for (auto x : a)
    {
        cnt[x]++;
    }
}

unsigned long long summator::get(size_t i) const
{
    return cnt[i];
}

