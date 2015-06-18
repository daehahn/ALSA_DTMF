#include "FileWriter.h"
#include <unistd.h>

using namespace std;

void FileWriter::execute(size_t num_items)
{
	if (!pool) return;
	if (num_items <= 0) return;
	this->num_items = num_items;

        file = fopen(file_name.c_str(), "w");

	start();
}

void FileWriter::run()
{
	if (!pool) return;
	for (size_t i = 0; i < num_items; ++i)
	{
		short *data = pool->pop();
		fwrite((char*)data, sizeof(short), TONE_BUFFER_SIZE, file);
	}
        fclose(file);
}

