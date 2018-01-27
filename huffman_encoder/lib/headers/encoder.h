//
// Created by Maxim on 12.01.2018.
//

#ifndef ENCODER_H
#define ENCODER_H

#include "huffman_tree.h"
#include "code.h"

struct encoder: private tree
{
    encoder() = delete;
    encoder(summator const &a);

    vector <unsigned char> get_digits() const;
    vector<pair<uint16_t, uint16_t>> get_tree() const;

    code encode(vector<unsigned char> const &data) const;

private:
    void digits_dfs(uint16_t v, vector<unsigned char> &a) const;
    void tree_dfs(uint16_t v, vector<pair<uint16_t, uint16_t>> &a) const;
};

#endif //ENCODER_H
