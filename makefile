CFLAGS = -g -I. -std=c++11 -m32 -L/home/joe/robocup/trunk/lib/linux32/
COMPILE = g++ $(CFLAGS) -c
LINK = g++ $(CFLAGS)
LIBS = -lstdc++ -lpthread -lasound

all: main

Common.o: Common.cpp Common.h
	$(COMPILE) Common.cpp -o Common.o

Thread.o: Thread.cpp Thread.h
	$(COMPILE) Thread.cpp -o Thread.o

Mutex.o: Mutex.cpp Mutex.h
	$(COMPILE) Mutex.cpp -o Mutex.o
	
Reader.o: Reader.cpp Reader.h Common.h Thread.h Pool.h Mutex.h
	$(COMPILE) Reader.cpp -o Reader.o

AlsaReader.o: AlsaReader.cpp AlsaReader.h Common.h Thread.h Pool.h Mutex.h
	$(COMPILE) AlsaReader.cpp -o AlsaReader.o

AlsaWriter.o: AlsaWriter.cpp AlsaWriter.h Common.h Thread.h Pool.h Mutex.h
	$(COMPILE) AlsaWriter.cpp -o AlsaWriter.o

FileWriter.o: FileWriter.cpp FileWriter.h Common.h Thread.h Pool.h Mutex.h
	$(COMPILE) FileWriter.cpp -o FileWriter.o

Detector.o: Detector.cpp Detector.h Common.h Thread.h Pool.h Mutex.h
	$(COMPILE) Detector.cpp -o Detector.o

main.o: main.cpp Pool.h Reader.h Detector.h Mutex.h Thread.h
	$(COMPILE) main.cpp -o main.o

main: Thread.o Mutex.o Common.o Reader.o AlsaReader.o AlsaWriter.o FileWriter.o Detector.o main.o
	$(LINK) main.o Thread.o Mutex.o Common.o Reader.o AlsaReader.o AlsaWriter.o FileWriter.o Detector.o $(LIBS) -o main

clean : 
	rm -f main *.o *~
