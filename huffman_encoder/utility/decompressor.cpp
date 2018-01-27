#include <iostream>
#include "decompressor.h"

vector <unsigned char> get_digits(size_t cnt, file_reader &reader)
{
	vector <unsigned char> ret;
	for (size_t i = 0; i < cnt; ++i)
	{
		if(reader.eof())
		{
			throw std::runtime_error("error");
		}
		ret.push_back(reader.get()); 
	}
	return ret;
}

vector <pair<uint16_t, uint16_t>> get_tree(size_t size, file_reader &reader)
{
	vector <pair<uint16_t, uint16_t>> ret(size);
	for (size_t i = 0; i < size; ++i)
	{
		try
		{
			ret[i].first = reader.get_short();
		}
		catch(...)
		{
			throw std::runtime_error("error");
		}
		try
		{
			ret[i].second = reader.get_short();
		}
		catch(...)
		{
			throw std::runtime_error("error");
		}
	}
	return ret;
}

void write_decompressed_block(vector<unsigned char> const &block, file_writer &writer)
{
	for (auto t : block)
	{
		writer.put(t);
	}
}

void decompress(string const &src, string const &dst)
{
	try
	{
		file_reader check(src);
	}
	catch(...)
	{
		throw std::runtime_error("error");
	}
	
	file_reader reader(src);
	file_writer writer(dst);
	size_t cnt = 0;

	try
	{
		cnt = static_cast<size_t>(reader.get_short());
	}
	catch(...)
	{
		throw std::runtime_error("error");
	}
	
	if(cnt < 2) throw std::runtime_error("error");
	
	vector <unsigned char> digits;
	try
	{
		digits = get_digits(cnt, reader);
	}
	catch(...) { throw; }

	vector <pair<uint16_t, uint16_t>> tree;
	try
	{
		tree = get_tree(cnt - 1, reader);
	}
	catch(...) { throw; }
	
	decoder decompressor;
	try
	{
		decompressor.init_decode(tree, digits);
	}
	catch(...) { throw; }

	vector <unsigned char> curr_block;
	while(!reader.eof())
	{
		size_t x, y;
		try
		{
			x = static_cast<size_t>(reader.get_short());
			y = static_cast<size_t>(reader.get_short());		
		}
		catch(...) { throw std::runtime_error("Cutted block."); }

		curr_block.resize(x);
		for (size_t i = 0; i < x; ++i)
		{
			if(reader.eof())
			{
				throw std::runtime_error("error");
			}
			curr_block[i] = reader.get();
		}
		write_decompressed_block(decompressor.decode(code(curr_block, y)), writer);
	}
}
