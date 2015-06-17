#ifndef __READER_H__
#define __READER_H__

#include "Common.h"
#include "Thread.h"
#include "Pool.h"
#include "ModuleBase.h"
#include <iostream>
#include <string>

using namespace std;

class Reader : public ModuleBase
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
		void execute(size_t);
};

#endif
