#ifndef __FILE_WRITER_H__
#define __FILE_WRITER_H__

#include "ModuleBase.h"
#include "Pool.h"
#include "Common.h"
#include <string>
#include <cstdio>

using namespace std;

class FileWriter : public ModuleBase
{
	private:
		Pool<short*> * pool;
		string id;
		string file_name;
		size_t num_items;
                FILE* file;
	protected:
		virtual void run();
	public:
		FileWriter(Pool<short *> * pool, string id, string file_name)
			: pool(pool), id(id), num_items(0), file_name(file_name){}
		void execute(size_t);
};

#endif
