#include "file_reader.h"

file_reader::file_reader(string const &filename) : reader(filename, ifstream::binary), name(filename), curr(0), end(0) 
{
	if(reader.fail())
	{
		reader.close();
		throw std::runtime_error("no file");
	}
}

file_reader::~file_reader()
{
	reader.close();
}

bool file_reader::eof()
{
	return (curr == end) && (reader.peek() == EOF);
}

unsigned char file_reader::get()
{
	if (curr == end)
	{
		reader.read(buffer, BUFFER_SIZE);
		end = reader.gcount();
		curr = 0;
	}
	return static_cast<unsigned char>(buffer[curr++]);
}

uint16_t file_reader::get_short()
{
	uint16_t ret = 0;
	if(eof())
	{
		throw std::runtime_error("Expected data.");
	}
	
	ret += static_cast<uint16_t>(get());
	ret <<= 8;
	
	if(eof())
	{
		throw std::runtime_error("Expected data.");
	}
	
	ret += static_cast<uint16_t>(get());
	return ret;
}
