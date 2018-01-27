#ifndef FILE_READER
#define FILE_READER

#include "consts.h"

#include <fstream>
#include <string>
#include <cstdlib>

using std::ifstream;
using std::string;

struct file_reader
{
	file_reader() = delete;
	~file_reader();	
	file_reader(string const &filename);
	
	bool eof();
	unsigned char get();
	uint16_t get_short();

private:
	
	ifstream reader;
	string name;
	char buffer[BUFFER_SIZE];
	size_t curr, end;
};


#endif //FILE_READER
