#ifndef _COMMON_H_
#define _COMMON_H_

#include <math.h>

#define PI_2 			(2 * M_PI)
#define SAMPLE_RATE 		48000
#define CHANNELS 		1
#define FRAME_SIZE 		(sizeof(short) * CHANNELS)
#define MARK 			100
#define SPACE 			100
#define NUM_MARK_SAMPLES 	((int)((MARK / 1000.0) * SAMPLE_RATE))
#define NUM_SPACE_SAMPLES 	((int)((SPACE / 1000.0) * SAMPLE_RATE))
#define FILTER_WIDTH 		(NUM_MARK_SAMPLES >> 1)
#define FILTER_STRIDE 		(FILTER_WIDTH >> 1)
#define ON_SAMPLES		5
#define ON_THRESHOLD		((ON_SAMPLES * 0.15))
#define TONE_BUFFER_SIZE 	((NUM_MARK_SAMPLES + NUM_SPACE_SAMPLES) * CHANNELS)
#define CODE_TO_CHAR(A, B)	((char)(((A) << 1) | (((B) << 1) << 4)))

class Common {
public:
  static double lower_freq[4];
  static double higher_freq[4];
  
  static short tone_buffer[4][4][TONE_BUFFER_SIZE];
  
  static char char_to_code[256][2];
  
  static char code_to_char[256];
 
  static double theta_lower[4]; 
   
  static double theta_higher[4]; 
  
  static double coef_low[4];
  
  static double coef_high[4];

  static double amp_low[4];

  static double amp_high[4];
  
  static void generate_tone_buffer();
  static void copy_tone_buffer(int i, int j, short*buf);
  static bool valid(char c);
  static void init();
  static void log();

};

#endif
