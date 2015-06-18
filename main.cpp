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
  ModuleBase *producer = NULL;
  ModuleBase *consumer = NULL;
 	Pool<short*> pool;

/*options:
  -i select input mode: (default is Reader)
     a -> AlsaReader
     r -> Reader
  -o select output mode: (default is Detector)
     a -> AlsaWriter
     d -> Detector
     f -> FileWriter
  -n specify number of items to process (default is 16)
  -f specify file name (for fileWriter, not implemented yet)
*/


  while((c = getopt(argc,argv,OPTS)) != -1){
    switch(c){
      case 'i':
        if(!strcmp(optarg,"a")){
          producer = new AlsaReader(&pool,"Alsa Reader");
        }else if(!strcmp(optarg,"r")){
          producer = new Reader(&pool,"Reader");
        }else{
          fprintf(stderr,"bad input option argument: %s\n",optarg);
          exit(EXIT_FAILURE);
        }
        break;
      case 'o':
        if(!strcmp(optarg,"a")){
          consumer = new AlsaWriter(&pool,"Alsa Writer");
        }else if(!strcmp(optarg,"d")){
          consumer = new Detector(&pool,"Freq Detector");
        }else if(!strcmp(optarg,"f")){
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

  if(producer == NULL) producer = new Reader(&pool,"Default Reader");
  if(consumer == NULL) consumer = new Detector(&pool,"Detector");

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

