#ifndef FILE_WRITER
#define FILE_WRITER

#include "consts.h"

#include <fstream>
#include <string>
#include <cstdlib>

using std::ofstream;
using std::string;

struct file_writer
{
	file_writer() = delete;
	~file_writer();	
	file_writer(string const &filename);
	
	void put(unsigned char const c);
	void put_short(uint16_t const x);

private:
	
	ofstream writer;
	char buffer[BUFFER_SIZE];
	size_t curr;
};


#endif //file_writer
