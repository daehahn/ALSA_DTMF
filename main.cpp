#include "Pool.h"
#include "Reader.h"
#include "Detector.h"
#include <string>

using namespace std;

int main(void)
{
	Pool<short*> pool;
	Reader prod(&pool, "Alsa Reader 1");
	Detector cons0(&pool, "Freq Detector 0");

	cons0.detect(10);
	prod.read(10);	

	prod.join();
	cons0.join();
	
	return 0;
}

