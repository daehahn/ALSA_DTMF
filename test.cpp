#include <cstdio>
#include <vector>
#include "RingBuffer.h"
#include <cmath>

double gaussian(double v, double std_dev)
{
  static double c = 1.0/sqrt(2.0*M_PI);
  return (c/std_dev) * exp(-0.5*pow(v/std_dev,2));
}


int main()
{
  const int nFreq = 8;
  const int dist = 8;
  const int length = 16;

  const int BUFFERSIZE = nFreq*dist*length*2;
  RingBuffer<double, BUFFERSIZE> buffer;
  while(buffer.getNumberOfEntries() != buffer.getMaxEntries())
    buffer.add(0);

  RingBuffer<double, BUFFERSIZE> buffer2;
  RingBuffer<double, 3> buffer3;



  double max[nFreq] = { 0 };
  RingBuffer<double, dist*length*10> maxBuffer[nFreq];
  

  //FILE *f = fopen("mag2.txt", "r");
  double mag;
  int p = 0;
  int t = 0;
  while(fscanf(stdin, "%lf", &mag) > 0)
  {
    buffer.add(mag); 
    if(mag > max[p])
      max[p] = mag;
    maxBuffer[p].add(mag);
    p = (p + 1) % nFreq;

    //-- after each row (at each cycle)
    if(p == 0)
    {
      t++;

      //-- add all magnitudes at potential peak positions
      double sum = 0;
      for(int j=0; j<length; j++)
        for(int f=0; f<nFreq; f++)
        {
          int p = (dist * j) * nFreq + f;
          //sum += buffer[p] / max[nFreq-1-f];
          double maxFromBuffer = 0;
          for(int i=0;i<maxBuffer[nFreq-1-f].getNumberOfEntries(); i++)
            if(maxFromBuffer < maxBuffer[nFreq-1-f][i])
              maxFromBuffer = maxBuffer[nFreq-1-f][i];
          sum += buffer[p] / maxFromBuffer;
        }
      buffer2.add(sum);

      //-- search for 5 peaks (the tip of the triangle)
      const int peaks = 5;
      if(buffer2.getNumberOfEntries() > peaks/2 * dist + 2 + dist*(length-1) )
      {
        double v[peaks];
        for(int i=0; i<peaks; i++)
          v[i] = buffer2[dist*i];

        //-- calc 
        double diff[peaks-1];
        double diffCombined = 1.0;
        for(int i=0; i<peaks-1; i++)
          diffCombined *= diff[i] = (v[i] - v[i+1]) * (i<peaks/2 ? -1.0 : 1.0);

        //-- evaluate symmetry
        double error = 0;
        for(int i=0; i<peaks/2-1; i++)
          error += fabs(diff[i] - diff[peaks-2-i]);
        
        double confidence = std::max(0.0, gaussian(error, 0.5) * diffCombined);
        buffer3.add(confidence);

        //printf("%lf\n", confidence);

        bool failed = false;
        if(buffer3[1] > buffer3[0] && buffer3[1] > buffer3[2] && buffer3[1] > 0.5)
        {
          int startPos = peaks/2 * dist + 2 + dist*(length-1);
          printf("message position %d\n", t - startPos);

          char data[length / 2 + 1] = { 0 };
          for(int i=0; i<length; i++)
          {
            int index1 = -1, index2 = -1;
            double max1 = 0, max2 = -1;
            for(int f=0; f<nFreq; f++)
            {
              int x = startPos*nFreq - i*dist*nFreq - f - 1;
              double v = buffer[x];
              if(max1 < max2 && v > max1)
              {
                max1 = v; 
                index1 = f;
              }
              else if(max2 < max1 && v > max2)
              {
                max2 = v; 
                index2 = f;
              }
            }
            //printf("%d %d   %lf %lf \n", index1, index2, max1, max2);
            
            if((index1 < nFreq/2) == (index2 < nFreq/2))
            {
              fprintf(stderr, "error!\n");
              failed = true;
            }


            int row = std::min(index1, index2);
            int col = std::max(index1, index2) - (nFreq/2);
            int tone = (row << 2) | col; 
            //printf("tone %d\n", tone);
            
            data[i/2] |= tone << (((i+1) & 1) * 4);
          }
          if(!failed)
            fprintf(stderr, "%s\n", data);
        }
      }
      else
        printf("0\n");      
    }
  }
  //fclose(f);

  return 0;
}
