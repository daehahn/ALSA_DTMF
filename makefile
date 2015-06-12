CFLAGS = -g -I.
COMPILE = g++ $(CFLAGS) -c
LINK = g++ $(CFLAGS)
LIBS = -lstdc++ -lpthread

all: main

Thread.o: Thread.cpp Thread.h
	$(COMPILE) Thread.cpp -o Thread.o

Mutex.o: Mutex.cpp Mutex.h
	$(COMPILE) Mutex.cpp -o Mutex.o
	
Reader.o: Reader.cpp Reader.h Thread.h Pool.h Mutex.h
	$(COMPILE) Reader.cpp -o Reader.o

Detector.o: Detector.cpp Detector.h Thread.h Pool.h Mutex.h
	$(COMPILE) Detector.cpp -o Detector.o

main.o: main.cpp Pool.h Reader.h Detector.h Mutex.h Thread.h
	$(COMPILE) main.cpp -o main.o

main: Thread.o Mutex.o Reader.o Detector.o main.o
	$(LINK) main.o Thread.o Mutex.o Reader.o Detector.o $(LIBS) -o main

clean : 
	rm -f main *.o *~
