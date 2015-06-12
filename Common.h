#ifndef _COMMON_H_
#define _COMMON_H_

#include <math.h>

const double PI_2 = 2.0 * M_PI;

const double lower_freq[4] = {697, 770, 852, 941};
const double higher_freq[4] = {1209, 1336, 1477, 1633};

const unsigned int SAMPLE_RATE = 48000;

unsigned int LOWER_AMP = 15000;
unsigned int HIGHER_AMP = 15000;

const unsigned int CHANNELS = 1;

const unsigned int FRAME_SIZE = sizeof(short) * CHANNELS; 

const double MARK = 60; //signal on for <mark> milliseconds
const double SPACE = 60; //signal off for <space> milliseconds

const int NUM_MARK_SAMPLES = (int)((MARK / 1000.0) * SAMPLE_RATE);
const int NUM_SPACE_SAMPLES = (int)((SPACE / 1000.0) * SAMPLE_RATE);

const int TONE_BUFFER_SIZE = NUM_MARK_SAMPLES * CHANNELS;

short tone_buffer[4][4][TONE_BUFFER_SIZE];

char code[256][2];

const double theta_lower[4]  = { (PI_2 / SAMPLE_RATE) * lower_freq[0],
			         (PI_2 / SAMPLE_RATE) * lower_freq[1],
			         (PI_2 / SAMPLE_RATE) * lower_freq[2],
			         (PI_2 / SAMPLE_RATE) * lower_freq[3]};

const double theta_higher[4] = { (PI_2 / SAMPLE_RATE) * higher_freq[0],
			         (PI_2 / SAMPLE_RATE) * higher_freq[1],
			         (PI_2 / SAMPLE_RATE) * higher_freq[2],
			         (PI_2 / SAMPLE_RATE) * higher_freq[3]};


double coef_lower[4] = { 2.0 * cos(theta_lower[0]),
			 2.0 * cos(theta_lower[1]),
			 2.0 * cos(theta_lower[2]),
			 2.0 * cos(theta_lower[3])};

double coef_higher[4] = { 2.0 * cos(theta_higher[0]),
			  2.0 * cos(theta_higher[1]),
			  2.0 * cos(theta_higher[2]),
			  2.0 * cos(theta_higher[3])};

#endif
