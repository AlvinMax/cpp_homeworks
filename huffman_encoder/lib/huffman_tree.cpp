//
// Created by Maxim on 12.01.2018.
//

#include "huffman_tree.h"

#include <set>
#include <stdexcept>
#include <utility>

using std::set;
using std::pair;
using std::vector;

tree::tree() {}

word reverse(word c)
{
    unsigned long long nc = 0;
    for(size_t i = 0; i < 64; ++i)
    {
        unsigned long long q = c.get(i);
        nc |= (q << (63 - i));
    }
    return word(nc, c.len);
}

tree::tree(summator const &a) : tree()
{
    set< pair<unsigned long long, uint16_t> > q;
    uint16_t cnt = 0;

    for(uint16_t i = 0; i < SYMB_RANGE + 1; ++i)
    {
        if(a.get(i) > 0)
        {
            q.insert(std::make_pair(a.get(i), cnt));
            nodes.push_back(node(i));
            cnt++;
        }
    }

    if(nodes.empty())
    {
        for(uint16_t i = 0; i < 2; ++i) {
            q.insert(std::make_pair(i + 0ULL, i));
            nodes.push_back(node(i));
            cnt++;
        }
    } else if(nodes.size() == 1)
    {
        q.insert(std::make_pair(a.get(nodes[0].dig), nodes[0].dig + 1));
        nodes.push_back(nodes[0].dig + 1);
        cnt++;
    }

    while(q.size() > 1)
    {
        unsigned long long cnt1 = q.begin()->first;
        uint16_t v1 = q.begin()->second;
        q.erase(q.begin());

        unsigned long long cnt2 = q.begin()->first;
        uint16_t v2 = q.begin()->second;
        q.erase(q.begin());

        nodes.push_back(node(v1, v2));
        q.insert(std::make_pair(cnt1 + cnt2, cnt));
        cnt++;
    }

    g.resize(cnt);

    uint16_t pos = 0;
    build_g(cnt - 1, pos);
    build_code(0, word());

    for(uint16_t i = 0; i < SYMB_RANGE + 1; ++i) {
        codes[i] = reverse(codes[i]);
    }

    nodes.clear();
    nodes.shrink_to_fit();

}

void tree::build(vector <pair<uint16_t, uint16_t>> const &p, vector <unsigned char> const &digits)
{
    if(p[0].first != 0) error();

    g.resize(2 * p.size() + 1);
    for(auto t: p) {
        if(t.first >= g.size() || t.second >= g.size() || t.first + 1 >= t.second) error();
        g[t.first].sons.resize(2);
        g[t.first].sons[0] = t.first + 1;
        g[t.first].sons[1] = t.second;
    }

    size_t j  = 0;
    for(size_t i = 0; i < g.size(); ++i) {
        if(g[i].sons.empty()) {
            if(j < digits.size()) g[i].dig = digits[j++];
            else error();
        } else if(g[i].sons.size() != 2) error();
    }

    if(j != digits.size()) error();

    uint16_t pos = 0;
    check(0, pos);
	word temp;
    build_code(0, temp);
}

void tree::build_g(uint16_t v, uint16_t &i)
{
    uint16_t curr = i;
    g[curr].dig = nodes[v].dig;
    if(!nodes[v].sons.empty()) {
        g[curr].sons.resize(2);
        for (unsigned j = 0; j < 2; ++j) {
            g[curr].sons[j] = ++i;
            build_g(nodes[v].sons[j], i);
        }
    }
}

void tree::check(uint16_t v, uint16_t &i)
{
    if(v != i) error();
    if(!g[v].sons.empty()) {
        for(size_t j = 0; j < 2; ++j) {
            ++i;
            check(g[v].sons[j], i);
        }
    }
}

void tree::build_code(uint16_t v, word c)
{
    if(g[v].dig != NULL_DIG)
    {
        codes[g[v].dig] = c;
    }
    for(size_t i = 0; i < g[v].sons.size(); ++i)
    {
        c.add(static_cast<bool>(i));
        build_code(g[v].sons[i], c);
        c.erase();
    }
}

void tree::error() const
{
    throw std::runtime_error("Bad tree.");
}
