CC = g++
CFLAGS = -I/usr/local/include -Wall -std=c++17
LDFLAGS= -ljack

all: tracolor

tracolor : jack_module.o delay.o reverse.o delayLine.o main.o
	$(CC) -o $@ $(CFLAGS) jack_module.o delay.o reverse.o delayLine.o main.o $(LDFLAGS)

.cpp.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o
	rm -f tracolor

clean_o:
	rm -f *.o
