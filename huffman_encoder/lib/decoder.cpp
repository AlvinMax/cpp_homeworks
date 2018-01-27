//
// Created by Maxim on 13.01.2018.
//

#include "decoder.h"

decoder::decoder() : tree(), flag(false) {}
void decoder::init_decode(std::vector<std::pair<uint16_t, uint16_t>> const &p, std::vector<unsigned char> const &digits)
{
    build(p, digits);
    flag = true;
}

vector <unsigned char> decoder::decode(code const &block) const
{
    if(!flag) throw std::runtime_error("No tree");

    uint16_t curr = 0;
    vector <unsigned char> res;
    res.reserve(block.size);

    for(size_t i = 0; i < block.digits.size(); ++i)
    {
        unsigned char t = block.digits[i];
        for(size_t j = 0; j < 8; ++j) {
            curr = g[curr].sons[static_cast<bool>(t&(1 << (7 - j)))];
            if(g[curr].dig != NULL_DIG) {
                res.push_back(static_cast<unsigned char>(g[curr].dig));
                curr = 0;
            }
        }
    }

    res.erase(res.begin() + block.size, res.end());
    return res;
}
