#include "file_writer.h"

file_writer::file_writer(string const &filename) : writer(filename, ofstream::binary), curr(0) {}

file_writer::~file_writer()
{
	writer.write(buffer, curr);
	writer.close();
}

void file_writer::put(unsigned char const c)
{
	if(curr == BUFFER_SIZE)
	{
		writer.write(buffer, curr);
		curr = 0;
	}
	buffer[curr++] = static_cast<char>(c);
}

void file_writer::put_short(uint16_t const x)
{
	put(static_cast<unsigned char>(x >> 8));
	put(static_cast<unsigned char>((x << 8) >> 8));
}
