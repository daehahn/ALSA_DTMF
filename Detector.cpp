#include "Detector.h"
#include <iostream>

using namespace std;

Mutex Detector::mtx;

void Detector::detect(size_t num_items)
{
	if (!pool) return;
	if (num_items <= 0) return;
	this->num_items = num_items;
	start();
}

void Detector::run()
{
	if (!pool) return;
	for (size_t i = 0; i < num_items; ++i)
	{
		short* data = pool->pop();
		//acquire lock to print to stdout
 		mtx.lock();
		cout << "[" << id << "] : <" << data[0] << ", " << data[1] 
			<< ", " << data[2] << ", " << data[3] <<  ">" << endl;
		mtx.unlock();

		delete[] data;
	}
}
