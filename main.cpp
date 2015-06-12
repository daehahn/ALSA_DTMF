#include "Common.h"
#include "Pool.h"
#include "AlsaReader.h"
#include "Reader.h"
#include "AlsaWriter.h"
#include "Detector.h"
#include <string>
#include <iostream>

using namespace std;

int main(void)
{
        Common::init();
        //Common::log();

 	Pool<short*> pool;
	//Reader prod(&pool, "Alsa Reader 1");
	AlsaReader prod1(&pool, "Alsa reader 2");
	//Detector cons0(&pool, "Freq Detector 0");
	AlsaWriter cons1(&pool, "Alsa Writer 0");

	//cons0.detect(2);
        cons1.write(100);
	prod1.read(100);	

	prod1.join();
	//cons0.join();
	cons1.join();
	
	return 0;
}

