#ifndef __READER_H__
#define __READER_H__

#include "Thread.h"
#include "Pool.h"
#include <string>

using namespace std;

class Reader : public Thread
{
	private:
		Pool<short*> * pool;
		string id;
		size_t num_items;
	protected:
		virtual void run();
	public:
		Reader(Pool<short *> * pool, string id)
			: pool(pool), id(id), num_items(0) {}
		void read(size_t);
};

#endif
