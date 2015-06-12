CFLAGS = -g -I. -std=c++11
COMPILE = g++ $(CFLAGS) -c
LINK = g++ $(CFLAGS)
LIBS = -lstdc++ -lpthread

all: main

Common.o: Common.cpp Common.h
	$(COMPILE) Common.cpp -o Common.o

Thread.o: Thread.cpp Thread.h
	$(COMPILE) Thread.cpp -o Thread.o

Mutex.o: Mutex.cpp Mutex.h
	$(COMPILE) Mutex.cpp -o Mutex.o
	
Reader.o: Reader.cpp Reader.h Common.h Thread.h Pool.h Mutex.h
	$(COMPILE) Reader.cpp -o Reader.o

Detector.o: Detector.cpp Detector.h Common.h Thread.h Pool.h Mutex.h
	$(COMPILE) Detector.cpp -o Detector.o

main.o: main.cpp Pool.h Reader.h Detector.h Mutex.h Thread.h
	$(COMPILE) main.cpp -o main.o

main: Thread.o Mutex.o Common.o Reader.o Detector.o main.o
	$(LINK) main.o Thread.o Mutex.o Common.o Reader.o Detector.o $(LIBS) -o main

clean : 
	rm -f main *.o *~
