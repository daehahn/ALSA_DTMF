#include "AlsaWriter.h"
#include <unistd.h>
#include <iostream>
#include <cstring>

using namespace std;

void AlsaWriter::write(size_t num_items)
{
	if (!pool) return;
	if (num_items <= 0) return;
	this->num_items = num_items;
	sample_rate = SAMPLE_RATE;
	alsa_setup("default",&sample_rate);
	start();
}

void AlsaWriter::run()
{
	int rc;
	if (!pool) return;
	for (size_t i = 0; i < num_items; ++i)
	{
		short *data = pool->pop();
		mtx.lock();
                rc = snd_pcm_writei(handle,data,TONE_BUFFER_SIZE/CHANNELS);
		mtx.unlock();
		//fwrite((char*)data, sizeof(short), TONE_BUFFER_SIZE, stdout);
		if(rc == -EPIPE){
			snd_pcm_prepare(handle);
		}
	}
        sleep(2);
}

void AlsaWriter::alsa_setup(const char *device,  unsigned int *sample_rate){
  int err;
  snd_pcm_hw_params_t *hw_params;

  if ((err = snd_pcm_open (&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    fprintf (stderr, "cannot open audio device %s (%s)\n",
                     device,
                     snd_strerror (err));
    exit (1);
  }

  if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
    fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
                     snd_strerror (err));
    exit (1);
  }

  if ((err = snd_pcm_hw_params_any (handle, hw_params)) < 0) {
    fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
                     snd_strerror (err));
    exit (1);
  }

  if ((err = snd_pcm_hw_params_set_access (handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    fprintf (stderr, "cannot set access type (%s)\n",
                     snd_strerror (err));
    exit (1);
  }

  if ((err = snd_pcm_hw_params_set_format (handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
    fprintf (stderr, "cannot set sample format (%s)\n",
                     snd_strerror (err));
    exit (1);
  }

  if ((err = snd_pcm_hw_params_set_rate_near (handle, hw_params, sample_rate, 0)) < 0) {
    fprintf (stderr, "cannot set sample rate (%s)\n",
                     snd_strerror (err));
    exit (1);
  }

  if ((err = snd_pcm_hw_params_set_channels (handle, hw_params, CHANNELS)) < 0) {
    fprintf (stderr, "cannot set channel count (%s)\n",
                     snd_strerror (err));
    exit (1);
  }

  if ((err = snd_pcm_hw_params (handle, hw_params)) < 0) {
    fprintf (stderr, "cannot set parameters (%s)\n",
                     snd_strerror (err));
    exit (1);
  }

  snd_pcm_hw_params_free (hw_params);

  if ((err = snd_pcm_prepare (handle)) < 0) {
    fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
                     snd_strerror (err));
    exit (1);
  }

}

