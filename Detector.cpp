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

        while(true) {

          while(index < TONE_BUFFER_SIZE) {
            double mag[8];
 
            for(int i = 0; i < 4; i++) {
              mag[i] = goertzel(buf, Common::coef_low[i], 
					index, FILTER_WIDTH);
	      mtx.lock();
              cout << (i == 0 ? "" : ", ") << mag[i];
              mtx.unlock();

            }
            for(int i = 0; i < 4; i++) {
              mag[i+4] = goertzel(buf, Common::coef_high[i], 
					index, FILTER_WIDTH);
	      mtx.lock();
              cout << ", " << mag[i+4];
              mtx.unlock();
            }

            mtx.lock();
            cout << endl;
            mtx.unlock();

	    index += (FILTER_STRIDE * CHANNELS);
	    
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

  double scale = length / 2.0;

  for(int i = x; i < x + length; i++) {
    q0 = (buf.get(i*CHANNELS) / 32767.0) + (coef * q1) - q2;
    q2 = q1;
    q1 = q0;
  }

  return (q1*q1 + q2*q2 - q1*q2*coef) / (scale*scale);
}
