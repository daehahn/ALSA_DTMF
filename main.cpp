#include "Common.h"
#include "Pool.h"
#include "Reader.h"
#include "Detector.h"
#include <string>
#include <iostream>

using namespace std;

int main(void)
{
        Common::init();
        //Common::log();

 	Pool<short*> pool;
	Reader prod(&pool, "Alsa Reader 1");
	Detector cons0(&pool, "Freq Detector 0");

	cons0.detect(2);
	prod.read(2);	

	prod.join();
	cons0.join();
	
	return 0;
}

