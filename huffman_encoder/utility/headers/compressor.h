#ifndef COMPRESSOR
#define COMPRESSOR

#include "file_reader.h"
#include "file_writer.h"
#include "encoder.h"

summator get_count(string const &filename) noexcept;
void write_compressed_block(code const &block, file_writer &writer);
void compress(string const &src, string const &dst);

#endif //COMPRESSOR
