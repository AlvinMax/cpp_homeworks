//
// Created by Maxim on 11.01.2018.
//

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "consts.h"
#include "summator.h"
#include "word.h"

using std::vector;
using std::pair;

struct tree
{
public:
    struct node
    {
        vector <uint16_t> sons;
        uint16_t dig;

        node() : sons(), dig(NULL_DIG) {}
        node(uint16_t dig) : sons(), dig(dig) {}
        node(uint16_t left, uint16_t right) : sons(2), dig(NULL_DIG) {
            sons[0] = left;
            sons[1] = right;
        }
    };

    tree();
    tree(summator const &a);

    vector <node> g, nodes;
    word codes[SYMB_RANGE + 1];

    void build(vector <pair<uint16_t, uint16_t>> const &p, vector <unsigned char> const &digits);
private:
    void build_g(uint16_t v, uint16_t &i);
    void build_code(uint16_t v, word c);
    void error() const;
    void check(uint16_t v, uint16_t &i);
};

#endif //HUFFMAN_TREE_H
