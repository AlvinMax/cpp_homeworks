//
// Created by Maxim on 13.01.2018.
//

#include "encoder.h"

using std::vector;
using std::pair;

encoder::encoder(summator const &a): tree(a) {}

void  encoder::digits_dfs(uint16_t v, vector <unsigned char> &a) const
{
    if(g[v].dig != NULL_DIG) a.push_back(static_cast<unsigned char>(g[v].dig));
    for(auto to : g[v].sons) digits_dfs(to, a);
}

void encoder::tree_dfs(uint16_t v, vector<pair<uint16_t, uint16_t>> &a) const
{
    if(!g[v].sons.empty()) a.push_back(std::make_pair(v, g[v].sons.back()));
    for(auto to : g[v].sons) tree_dfs(to, a);
}

vector <unsigned char> encoder::get_digits() const
{
    vector <unsigned char> ret;
    digits_dfs(0, ret);
    return ret;
}

vector <pair<uint16_t, uint16_t>> encoder::get_tree() const
{
    vector <pair<uint16_t, uint16_t>> ret;
    tree_dfs(0, ret);
    return ret;
}

code encoder::encode(vector <unsigned char> const &data) const
{
    vector <unsigned long long> t(data.size(), 0ULL);
    size_t curr = 0;
    for(auto x : data) {
        word c  = codes[x];
        t[curr >> 6] |= c.code >> (curr & 63);
        size_t added = std::min(c.len, 64 - (curr & 63));
        curr += added;

        t[curr >> 6] |= c.code << added;
        curr += c.len - added;
    }

    vector <unsigned char> res(8 * t.size());
    for(size_t i = 0; i < t.size(); ++i)
    {
        for(size_t j = 0; j < 8; ++j)
        {
            res[i*8+j] = static_cast<unsigned char>(((t[i] >> (8 * (7 - j))) << (64 - 8)) >> (64 - 8));
        }
    }
    res.resize((curr >> 3) + static_cast<size_t>((curr & 7) != 0));
    return code(res, data.size());
}
