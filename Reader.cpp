#include "Reader.h"
#include <unistd.h>
#include <iostream>
#include <cstring>

using namespace std;

void Reader::read(size_t num_items)
{
	if (!pool) return;
	if (num_items <= 0) return;
	this->num_items = num_items;
	start();
}

void Reader::run()
{
	if (!pool) return;
	for (size_t i = 0; i < num_items; ++i)
	{
                short stack_data[4] = {i, i, i, i};
		short * data = new short[4];

		memcpy(data, &stack_data[0], sizeof(short) * 4);

		pool->push(data);
		usleep(100);
	}
}
