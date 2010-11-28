#This is the Makefile for out RTX in LINUX cmd
#run our OS by typing make

CFLAGS = -c -Wall
CC   = gcc
LDFLAGS=
Targets = startInitializeProcess.o startMessageEnvelope.o startSignalHandling.o startForkAuxillary.o iProcesses.o kPrimitives.o UserProcesses.o UserAPI.o Main.o


.PHONY: all

all: clean  primitiveRTX UNIXcrt.o UNIXkbd.o

clean:
	@echo "Cleaning object files.."
	rm -rf $(Targets) primitiveRTX UNIXcrt.o UNIXkbd.o
	@echo "..object files removed."

primitiveRTX: $(Targets)
	@echo "Compiling primitiveRTX"
	gcc -o primitiveRTX $(Targets)

UNIXcrt.o: UNIXcrt.c
	@echo "Compiling UNIXcrt"
	gcc $(CFLAGS) UNIXcrt.c
	
UNIXkbd.o: UNIXkbd.c
	@echo "Compiling UNIXkbd"
	gcc $(CFLAGS) UNIXkbd.c
