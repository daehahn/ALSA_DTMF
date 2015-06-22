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

        vector<unsigned char> message;

        TwoBuffer buf(a, b, TONE_BUFFER_SIZE, TONE_BUFFER_SIZE);

        if(num_items < 2) return;

        int items_consumed = 2;

        int index = 0;

        double mag[8][ON_SAMPLES];
        double dmag[8][ON_SAMPLES];
        int tone_on[8][ON_SAMPLES];
        double sum[8];
        double mean[8];
        double std[8];
/*
        FILE* mag_file = fopen("mag.txt", "w");
        FILE* dmag_file = fopen("dmag.txt", "w");
        FILE* mean_file = fopen("mean.txt", "w");
        FILE* std_file = fopen("std.txt", "w");
*/
        for(int i = 0; i < 8; i++) {
          sum[i] = 0;
        }

        for(int i = 0; i < 8; i++) {
          for(int j = 0; j < ON_SAMPLES; j++) {
            mag[i][j] = 0;
            dmag[i][j] = 0;
            tone_on[i][j] = 0;
          }
        }

        int mag_index = 0;
        int off_count = 0;

        while(true) {

          while(index < TONE_BUFFER_SIZE) {
 
            int before = (mag_index < 2 ? ON_SAMPLES - 2 : mag_index-2);
            int after = mag_index;


            for(int i = 0; i < 4; i++) {
              mag[i][mag_index] = goertzel(buf, Common::coef_low[i], 
					index, FILTER_WIDTH);
		
	      double ddf = -1 * mag[i][before] + mag[i][after];
              dmag[i][mag_index] = ddf;
              tone_on[i][mag_index] = dmag[i][before] > 0.0001 && dmag[i][after] < -0.0001 ? 1 : 0;

/*
	      fprintf(mag_file, "%s %g", i == 0 ? "" : ", ", mag[i][mag_index]);
	      fprintf(dmag_file, "%s %g", i == 0 ? "" : ", ", dmag[i][mag_index]);
	      fprintf(mean_file, "%s %g", i == 0 ? "" : ", ", mean[i]);
	      fprintf(std_file, "%s %g", i == 0 ? "" : ", ", std[i]);
*/
              //cout << (i == 0 ? "" : " ") << mag[i][mag_index];
	      //cout << (i == 0 ? "" : ", ") << ddf;

            }
            for(int i = 0; i < 4; i++) {
              mag[i+4][mag_index] = goertzel(buf, Common::coef_high[i], 
					index, FILTER_WIDTH);
		
	      double ddf = -1 * mag[i+4][before] + mag[i+4][after];
              dmag[i+4][mag_index] = ddf;
              tone_on[i+4][mag_index] = dmag[i+4][before] > 0.0001 && dmag[i+4][after] < -0.0001 ? 1 : 0;
/*
	      fprintf(mag_file, "%s %g", ", ", mag[i+4][mag_index]);
	      fprintf(dmag_file, "%s %g", ", ", dmag[i+4][mag_index]);
              fprintf(mean_file, "%s %g", ", ", mean[i+4]);
	      fprintf(std_file, "%s %g", ", ", std[i+4]);
  */          
	      //cout << ", " << ddf;

              //cout << " " << mag[i+4][mag_index];
            }
/*
            fprintf(mag_file, "\n");
            fprintf(dmag_file, "\n");
            fprintf(mean_file, "\n");
            fprintf(std_file, "\n");
*/
            
            //cout << endl;

            int prev = (mag_index >= 1 ? mag_index-1 : ON_SAMPLES-1);
 
            vector<int> low_index_prev;
            vector<int> high_index_prev;
            vector<int> low_index;
            vector<int> high_index;

            for(int i = 0; i < 4; i++) {
              if(tone_on[i][prev] == 1) low_index_prev.push_back(i);
              if(tone_on[i][mag_index] == 1) low_index.push_back(i);
            }

            for(int i = 4; i < 8; i++) {
              if(tone_on[i][prev] == 1) high_index_prev.push_back(i);
              if(tone_on[i][mag_index] == 1) high_index.push_back(i);
            }

            //cout << endl;

            int x = -1;
            int y = -1;

            if(low_index.size() == 1 && high_index.size() == 1) {
              x = low_index[0];
              y = high_index[0] - 4;
            } else if(low_index.size() == 1 && high_index_prev.size() == 1) {
              x = low_index[0];
              y = high_index_prev[0] - 4;
            } else if(low_index_prev.size() == 1 && high_index.size() == 1) {
              x = low_index_prev[0];
              y = high_index[0] - 4;
            }

            if(x != -1 && y != -1) {
              //printf("off_count: %d\n", off_count);
              //printf("on\n");
              if(off_count >= 3) {
                mtx.lock();
                unsigned char c = (unsigned char)(4*x + y);
                message.push_back(c);
                //cout << "Detected " << Common::code_to_char[CODE_TO_CHAR(low_index[0], high_index[0])] << endl;
                mtx.unlock();
              }
              off_count = 0;
            } else {
              //printf("off\n");
              off_count++;
            }

            mtx.lock();
            //cout << endl;
            mtx.unlock();

	    index += (FILTER_STRIDE * CHANNELS);
            //printf("index: %d\n", index);
            //for(int i = 0; i < 8; i++) {
              //sum[i] -= mag[i][(mag_index+1)%ON_SAMPLES];
            //}
            mag_index = (mag_index + 1) % ON_SAMPLES;
            //printf("mag index: %d\n", mag_index);

          }

	  if(items_consumed >= num_items) {
	    //printf("items consumed: %d\n", items_consumed);
            //printf("num items: %d\n", num_items);
            printf("message size: %d characters\n", message.size() / 2);

            for(int i = 0; i < message.size(); i += 2) {
              unsigned char c = (unsigned char)(((message[i] << 4) & 0xF0) 						  | (message[i+1] & 0x0F));
              cout << c;
            }
            cout << endl;

           
            //fclose(mag_file);
            //fclose(dmag_file);
            //fclose(mean_file);
            //fclose(std_file);

            return;
          }


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
