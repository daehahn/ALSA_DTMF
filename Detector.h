#ifndef __DETECTOR_H__
#define __DETECTOR_H__

#include "Thread.h"
#include "Pool.h"
#include <string>

using namespace std;

class Detector : public Thread
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
		void detect(size_t);
};

#endif
