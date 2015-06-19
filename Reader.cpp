#include "Reader.h"
#include <unistd.h>
#include <iostream>
#include <cstring>

using namespace std;

void Reader::execute(size_t num_items)
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
		unsigned char input = getchar();
		unsigned char first_half = (input & 0xF0) >> 4;
		unsigned char second_half = (input & 0x0F);
		
		short * data = new short[TONE_BUFFER_SIZE];
	  cout << "sending: " << Common::code_to_char[CODE_TO_CHAR(first_half/4,first_half%4)] << endl;
                Common::copy_tone_buffer((first_half/4), first_half%4, data);
                //Common::copy_tone_buffer((i/4)%4, i%4, data);
		for(int j = NUM_MARK_SAMPLES * CHANNELS; 
					j < TONE_BUFFER_SIZE; j++) {
                  data[i] = 0;
                }

		pool->push(data);

                data = new short[TONE_BUFFER_SIZE];

	  cout << "sending: " << Common::code_to_char[CODE_TO_CHAR(second_half/4,second_half%4)] << endl;
                Common::copy_tone_buffer((second_half/4), second_half%4, data);
		for(int i = NUM_MARK_SAMPLES * CHANNELS; 
				i < TONE_BUFFER_SIZE; i++) {
                 data[i] = 0;
               }

		pool->push(data);
	}
}
