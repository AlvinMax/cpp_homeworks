//
// Created by Maxim on 12.01.2018.
//

#ifndef DECODER_H
#define DECODER_H

#include "huffman_tree.h"
#include "code.h"

struct decoder: private tree
{
    decoder();

    void init_decode(vector<pair<uint16_t, uint16_t>> const &p, vector <unsigned char> const &digits);
    vector <unsigned char> decode(code const &block) const;

private:
    bool flag;
};

#endif //DECODER_H
