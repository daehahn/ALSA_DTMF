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

		short * data = new short[TONE_BUFFER_SIZE];

                Common::copy_tone_buffer(1, 1, data);
		for(int i = NUM_MARK_SAMPLES * CHANNELS; 
					i < TONE_BUFFER_SIZE; i++) {
                  data[i] = 0;
                }

		pool->push(data);
		usleep(100);
	}
}
