#include <iostream>
#include "compressor.h"
#include "decompressor.h"
#include <ctime>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 4)
	{
		cerr << "Uncorrest number of arguments\n";
		cerr << "Expected foramt: -mode(-c or -d) SRC DST\n";
		return 0;
	}
	string const mode = argv[1];
	string const SRC = argv[2];
	string const DST = argv[3];
	unsigned long long t1, t2;
	if (mode == "-c")
	{
		cerr << "Compressing...\n";
		t1 = clock();
		try
		{
			compress(SRC, DST);
		}
		catch (std::runtime_error const e)
		{
			cerr << "Failed to compress: " << SRC << endl;
			cerr << e.what();
			return 0;
		}
		t2 = clock();
		cerr << "Successfully compresed in " << double(t2 - t1) / CLOCKS_PER_SEC << " seconds\n";
	}
	else if (mode == "-d")
	{
		cerr << "Decompressing...\n";
		t1 = clock();
		try
		{
			decompress(SRC, DST);
		}
		catch (std::runtime_error const e)
		{
			cerr << "Failed to decompress: " << SRC << endl;
			cerr << e.what();
			return 0;
		}
		t2 = clock();
		cerr << "Successfully decompresed in " << double(t2 - t1) / CLOCKS_PER_SEC << " seconds\n";
	}
	else
	{
		cerr << "Uncorrect mode\nTry -c or -d";
	}
	return 0;
}
