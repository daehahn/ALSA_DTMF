#ifndef __ALSA_WRITER_H__
#define __ALSA_WRITER_H__

#include "Thread.h"
#include "Pool.h"
#include "Common.h"
#include "ModuleBase.h"
#include <string>
#include <alsa/asoundlib.h>
using namespace std;

class AlsaWriter : public ModuleBase
{
	private:
		Pool<short*> * pool;
		string id;
		size_t num_items;
                snd_pcm_t *handle;
                void alsa_setup(const char *device, unsigned int *sample_rate);
		unsigned int sample_rate;
	protected:
		virtual void run();
	public:
		AlsaWriter(Pool<short *> * pool, string id)
			: pool(pool), id(id), num_items(0) {}
		void execute(size_t);
};

#endif
