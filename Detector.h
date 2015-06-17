#ifndef __DETECTOR_H__
#define __DETECTOR_H__

#include "Common.h"
#include "Thread.h"
#include "Pool.h"
#include "ModuleBase.h"
#include <string>

using namespace std;

class TwoBuffer {
public:
  TwoBuffer() : a(NULL), b(NULL), la(-1), lb(-1) {} 

  TwoBuffer(short*a, short*b, int la, int lb) : 
			a(a), b(b), la(la), lb(lb){}

  ~TwoBuffer() {
     if(a) delete[] a;
     if(b) delete[] b;
   }

  short get(int i) {
    if(i < 0 || i >= la + lb) return -1;
    if(i < la) return a[i];
    else       return b[i - la];
  }

  void swap_out(short* c, int lc) {
    short* temp = a;
    a = b;
    b = c;
    la = lb;
    lb = lc;
    if(temp) delete[] temp;
  }

private:
  short* a;
  short* b;
  int la;
  int lb;
};

class Detector : public ModuleBase
{
	private:
		static Mutex mtx;
		Pool<short*> * pool;
		string id;
		size_t num_items;
	protected:
		virtual void run();
	public:
		Detector(Pool<short*> * pool, string id)
			: pool(pool), id(id), num_items(0) {}
		void execute(size_t);
		double goertzel(TwoBuffer &buf, double coef, 
						int x, int length);

};

#endif
