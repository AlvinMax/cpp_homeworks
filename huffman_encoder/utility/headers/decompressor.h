#ifndef DECOMPRESSOR
#define DECOMPRESSOR

#include "file_reader.h"
#include "file_writer.h"
#include "decoder.h"

vector <unsigned char> get_digits(size_t cnt, file_reader &reader);
vector <pair<uint16_t, uint16_t>> get_tree(size_t size, file_reader &reader);
void write_decommpressed_block(vector <unsigned char> const &block, file_writer &writer);
void decompress(string const &src, string const &dst);

#endif //DECOMPRESSOR
