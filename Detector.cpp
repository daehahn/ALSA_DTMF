#include "Detector.h"
#include <iostream>

using namespace std;

Mutex Detector::mtx;

void Detector::execute(size_t num_items)
{
	if (!pool) return;
	if (num_items <= 0) return;
	this->num_items = num_items;
	start();
}

void Detector::run()
{
	if (!pool) return;
 
   
        short* a = pool->pop();
        short* b = pool->pop();

        TwoBuffer buf(a, b, TONE_BUFFER_SIZE, TONE_BUFFER_SIZE);

        if(num_items < 2) return;

        int items_consumed = 2;

        int index = 0;

        double mag[8][ON_SAMPLES];
        double sum[8];
 
        for(int i = 0; i < 8; i++) {
          sum[i] = 0;
        }

        for(int i = 0; i < 8; i++) {
          for(int j = 0; j < ON_SAMPLES; j++) {
            mag[i][j] = 0;
          }
        }

        int mag_index = 0;
        bool on = false;

        while(true) {

          while(index < TONE_BUFFER_SIZE) {
 
            for(int i = 0; i < 4; i++) {
              mag[i][mag_index] = goertzel(buf, Common::coef_low[i], 
					index, FILTER_WIDTH);
	      mtx.lock();
	      int before = (mag_index == 0 ? ON_SAMPLES - 1 : mag_index-1);
              int after = (mag_index + 1) % ON_SAMPLES;
		
	      double ddf = -1 * mag[i][before] + mag[i][after];

              //cout << (i == 0 ? "" : ", ") << mag[i][mag_index];
	      cout << (i == 0 ? "" : ", ") << ddf;
              mtx.unlock();

            }
            for(int i = 0; i < 4; i++) {
              mag[i+4][mag_index] = goertzel(buf, Common::coef_high[i], 
					index, FILTER_WIDTH);
	      mtx.lock();
              int before = (mag_index == 0 ? ON_SAMPLES - 1 : mag_index-1);
              int after = (mag_index + 1) % ON_SAMPLES;
		
	      double ddf = -1 * mag[i+4][before] + mag[i+4][after];

	      cout << ", " << ddf;

              //cout << ", " << mag[i+4][mag_index];
              mtx.unlock();
            }

            
            vector<int> low_index;
            vector<int> high_index;

            for(int i = 0; i < 4; i++) {
              sum[i] += mag[i][mag_index];
              //cout << (i == 0 ? "" : ", ") << sum[i];
              if(sum[i] >= (ON_THRESHOLD)) {
                low_index.push_back(i);
              }
            }
 
            for(int i = 4; i < 8; i++) {
              sum[i] += mag[i][mag_index];
              //cout << ", " << sum[i];
              if(sum[i] >= (ON_THRESHOLD)) {
                high_index.push_back(i - 4);
              }
            }
 

            if(low_index.size() == 1 && high_index.size() == 1 && !on) {
              mtx.lock();
              //cout << "Detected" << Common::code_to_char[CODE_TO_CHAR(low_index[0], high_index[0])] << endl;
              mtx.unlock();
              on = true;
            } else{
              on = false;
            }
             

            mtx.lock();
            cout << endl;
            mtx.unlock();

	    index += (FILTER_STRIDE * CHANNELS);
            for(int i = 0; i < 8; i++) {
              sum[i] -= mag[i][(mag_index+1)%ON_SAMPLES];
            }
            mag_index = (mag_index + 1) % ON_SAMPLES;

          }

	  if(items_consumed >= num_items) return;

	  index -= TONE_BUFFER_SIZE;

	  short* c = pool->pop();
          buf.swap_out(c, TONE_BUFFER_SIZE);
	  items_consumed++;
	
        }

}

double Detector::goertzel(TwoBuffer &buf, double coef, int x, int length) {

  double q0, q1, q2;
  q0 = q1 = q2 = 0;

  double scale = length;

  double min = 100000;
  double max = -100000;
  double val;

  for(int i = x; i < x + length; i++) {
    val = buf.get(i*CHANNELS);
    min = (val < min ? val : min);
    max = (val > max ? val : max);
  }

  for(int i = x; i < x + length; i++) {
    q0 = ((buf.get(i*CHANNELS) - min) / (max - min)) + (coef * q1) - q2;
    q2 = q1;
    q1 = q0;
  }

  return ((q1*q1 + q2*q2 - q1*q2*coef) / (scale*scale)) * 66.6;
}
