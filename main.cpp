#include "Common.h"
#include "Pool.h"
#include "AlsaReader.h"
#include "Reader.h"
#include "AlsaWriter.h"
#include "Detector.h"
#include <stdlib.h>
#include <string>
#include <iostream>

#define OPTS "i:o:f:n:"

using namespace std;

int main(int argc, char ** argv)
{
  int n = 16;
  char c;
  Common::init();
  //Common::log();
  ModuleBase *producer;
  ModuleBase *consumer;
 	Pool<short*> pool;

  while((c = getopt(argc,argv,OPTS)) != -1){
    switch(c){
      case 'i':
        if(optarg[0] == 'a'){
          producer = new AlsaReader(&pool,"Alsa Reader");
        }else if(optarg[0] == 'r'){
          producer = new Reader(&pool,"Reader");
        }else{
          fprintf(stderr,"bad input option argument: %s\n",optarg);
          exit(EXIT_FAILURE);
        }
        break;
      case 'o':
        if(optarg[0] == 'a'){
          consumer = new AlsaWriter(&pool,"Alsa Writer");
        }else if(optarg[0] == 'd'){
          consumer = new Detector(&pool,"Freq Detector");
        }else if(optarg[0] == 'f'){
//          consumer = new FileWriter(&pool,"File Writer");
        }else{
          fprintf(stderr,"invalid output option argument: %s\n",optarg);
          exit(EXIT_FAILURE);
        }
        break;
      case 'n':
        n = atoi(optarg);
        break;
      case 'f':
        
        break;
    }
  }

	//Reader prod(&pool, "Alsa Reader 1");
	//AlsaReader prod1(&pool, "Alsa reader 2");
	//Detector cons0(&pool, "Freq Detector 0");
	//AlsaWriter cons1(&pool, "Alsa Writer 0");
  
  producer->execute(n);
  consumer->execute(n);

	//cons0.detect(55);
  //cons1.execute(16);
	//prod.execute(16);	

	//prod.join();
	//cons0.join();
	//cons1.join();
  producer->join();
  consumer->join();
	
	return 0;
}

