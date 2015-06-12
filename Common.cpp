#include "Common.h"
#include <iostream>
#include <cstring>

using namespace std;

double Common::lower_freq[4];
double Common::higher_freq[4];

short Common::tone_buffer[4][4][TONE_BUFFER_SIZE];

char Common::char_to_code[256][2];
char Common::code_to_char[256];

double Common::theta_lower[4];
double Common::theta_higher[4];
double Common::coef_low[4];
double Common::coef_high[4];

void Common::generate_tone_buffer() {

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {

      double f1 = lower_freq[i];
      double f2 = higher_freq[j];

      int k, x;

      for(k = 0, x = 0; k < NUM_MARK_SAMPLES * CHANNELS; k += 2, x++) {
        tone_buffer[i][j][k] =
          (short)(LOWER_AMP * sin(2 * M_PI * ((x * f1) / SAMPLE_RATE)));

        tone_buffer[i][j][k] +=
          (short)(HIGHER_AMP * sin(2 * M_PI * ((x * f2) / SAMPLE_RATE)));

        tone_buffer[i][j][k+1] = tone_buffer[i][j][k];
      }

    }
  }

}

void Common::copy_tone_buffer(int i, int j, short *buf) {

  memcpy((char*)buf, (char*)tone_buffer[i][j],
                NUM_MARK_SAMPLES * CHANNELS * sizeof(short));
}

bool Common::valid(char c) {

  switch(c) {
    case '1': case '2': case '3': case 'A':
    case '4': case '5': case '6': case 'B':
    case '7': case '8': case '9': case 'C':
    case '*': case '0': case '#': case 'D':
      return true;
    default:
      return false;
  }

}

void Common::init() {

  char_to_code['1'][0] = 0; char_to_code['1'][1] = 0;
  char_to_code['2'][0] = 0; char_to_code['2'][1] = 1;
  char_to_code['3'][0] = 0; char_to_code['3'][1] = 2;
  char_to_code['A'][0] = 0; char_to_code['A'][1] = 3;
  char_to_code['4'][0] = 1; char_to_code['4'][1] = 0;
  char_to_code['5'][0] = 1; char_to_code['5'][1] = 1;
  char_to_code['6'][0] = 1; char_to_code['6'][1] = 2;
  char_to_code['B'][0] = 1; char_to_code['B'][1] = 3;
  char_to_code['7'][0] = 2; char_to_code['7'][1] = 0;
  char_to_code['8'][0] = 2; char_to_code['8'][1] = 1;
  char_to_code['9'][0] = 2; char_to_code['9'][1] = 2;
  char_to_code['C'][0] = 2; char_to_code['C'][1] = 3;
  char_to_code['*'][0] = 3; char_to_code['*'][1] = 0;
  char_to_code['0'][0] = 3; char_to_code['0'][1] = 1;
  char_to_code['#'][0] = 3; char_to_code['#'][1] = 2;
  char_to_code['D'][0] = 3; char_to_code['D'][1] = 3;

  code_to_char[CODE_TO_CHAR(0, 0)] = '1';
  code_to_char[CODE_TO_CHAR(0, 1)] = '2';
  code_to_char[CODE_TO_CHAR(0, 2)] = '3';
  code_to_char[CODE_TO_CHAR(0, 3)] = 'A';
  code_to_char[CODE_TO_CHAR(1, 0)] = '4';
  code_to_char[CODE_TO_CHAR(1, 1)] = '5';
  code_to_char[CODE_TO_CHAR(1, 2)] = '6';
  code_to_char[CODE_TO_CHAR(1, 3)] = 'B';
  code_to_char[CODE_TO_CHAR(2, 0)] = '7';
  code_to_char[CODE_TO_CHAR(2, 1)] = '8';
  code_to_char[CODE_TO_CHAR(2, 2)] = '9';
  code_to_char[CODE_TO_CHAR(2, 3)] = 'C';
  code_to_char[CODE_TO_CHAR(3, 0)] = '*';
  code_to_char[CODE_TO_CHAR(3, 1)] = '0';
  code_to_char[CODE_TO_CHAR(3, 2)] = '#';
  code_to_char[CODE_TO_CHAR(3, 3)] = 'D';

  lower_freq[0] = 697;
  lower_freq[1] = 770;
  lower_freq[2] = 852;
  lower_freq[3] = 941;

  higher_freq[0] = 1209;
  higher_freq[1] = 1336;
  higher_freq[2] = 1477;
  higher_freq[3] = 1633;

  for(int i = 0; i < 4; i++) {
    theta_lower[i]  = (PI_2 / SAMPLE_RATE) *  lower_freq[i];
    theta_higher[i] = (PI_2 / SAMPLE_RATE) * higher_freq[i];
    coef_low[i]	    = 2.0 * cos(theta_lower[i]);
    coef_high[i]    = 2.0 * cos(theta_higher[i]);
  }
 
  generate_tone_buffer(); 
}

void Common::log() {
  cout << endl;
  cout << "Common Parameters-------------------------------" << endl;
  cout << "  SAMPLE_RATE       : " << SAMPLE_RATE << endl;
  cout << "  LOWER_AMP         : " << LOWER_AMP << endl;
  cout << "  HIGHER_AMP        : " << HIGHER_AMP << endl;
  cout << "  CHANNELS          : " << CHANNELS << endl;
  cout << "  FRAME_SIZE(bytes) : " << FRAME_SIZE << endl;
  cout << "  MARK(ms)          : " << MARK << endl;
  cout << "  SPACE(ms)         : " << SPACE << endl;
  cout << "  NUM_MARK_SAMPLES  : " << NUM_MARK_SAMPLES << endl;
  cout << "  NUM_SPACE_SAMPLES : " << NUM_SPACE_SAMPLES << endl;
  cout << "  FILTER_WIDTH      : " << FILTER_WIDTH << endl;
  cout << "  FILTER_STRIDE     : " << FILTER_STRIDE << endl;
  cout << "  TONE_BUFFER_SIZE  : " << TONE_BUFFER_SIZE << endl;
  cout << "  theta_lower[" << lower_freq[0] << "]: " 
			  << theta_lower[0] << endl;
  cout << "  theta_lower[" << lower_freq[1] << "]: " 
			  << theta_lower[1] << endl;
  cout << "  theta_lower[" << lower_freq[2] << "]: " 
			  << theta_lower[2] << endl;
  cout << "  theta_lower[" << lower_freq[3] << "]: " 
			  << theta_lower[3] << endl;
  cout << "  theta_higher[" << higher_freq[0] << "]: " 
			  << theta_higher[0] << endl;
  cout << "  theta_higher[" << higher_freq[1] << "]: " 
			  << theta_higher[1] << endl;
  cout << "  theta_higher[" << higher_freq[2] << "]: " 
			  << theta_higher[2] << endl;
  cout << "  theta_higher[" << higher_freq[3] << "]: " 
			  << theta_higher[3] << endl;

  cout << endl;
}
