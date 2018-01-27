#include <iostream>
#include "compressor.h"

summator get_count(string const &filename) noexcept
{
	summator ret;
	file_reader reader(filename);
	vector <unsigned char> block;
	while(!reader.eof()) 
	{
		if(block.size() == BLOCK_SIZE)
		{
			ret.add(block);
			block.clear();
		}
		block.push_back(reader.get());
	}
	ret.add(block);
	return ret;
}

void write_compressed_block(code const &block, file_writer &writer)
{
	writer.put_short(static_cast<uint16_t>(block.digits.size()));
	writer.put_short(static_cast<uint16_t>(block.size));
	for (auto t : block.digits)
	{
		writer.put(t);
	}
}

void compress(string const &src, string const &dst)
{
	try 
	{
		file_reader check(src);
	}
	catch (...)
	{
		throw std::runtime_error("Try to compress other file.");
	}

	encoder compressor(get_count(src));
	file_reader reader(src);
	file_writer writer(dst);

	vector <unsigned char> digits(compressor.get_digits());
	vector <pair<uint16_t, uint16_t>> tree(compressor.get_tree());

	writer.put_short(static_cast<uint16_t>(digits.size()));
	
	for(auto dig : digits)
	{
		writer.put(dig);
	}
	for(auto t : tree)
	{
		writer.put_short(t.first);
		writer.put_short(t.second);
	}

	vector <unsigned char> block;
	block.reserve(BLOCK_SIZE);
	while(!reader.eof())
	{
		if(block.size() == BLOCK_SIZE)
		{
			write_compressed_block(compressor.encode(block), writer);
			block.clear();
			block.reserve(BLOCK_SIZE);
		}
		block.push_back(reader.get());
	}

	if(!block.empty())
	{
		write_compressed_block(compressor.encode(block), writer);
	}
}
