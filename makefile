#This is the Makefile for out RTX in LINUX cmd
#run our OS by typing make

CFLAGS = -c -Wall
CC = gcc
LDFLAGS=
Targets = startInitializeProcess.o startMessageEnvelope.o startSignalHandling.o startForkAuxillary.o iProcesses.o kPrimitives.o UserProcesses.o UserAPI.o Main.o
LNFLAGS = -g
LIBS = -lcurses

.PHONY: all

all: clean primitiveRTX.o UNIXcrt.o UNIXkbd.o cleanafter

clean: 
	rm -rf $(Targets) primitiveRTX.o UNIXcrt.o UNIXcrttemp.o UNIXkbd.o UNIXkbdtemp.o

cleanafter:
	rm -rf $(Targets) UNIXcrttemp.o UNIXkbdtemp.o

primitiveRTX.o: $(Targets)
	gcc $(LNFLAGS) -o $@ $(Targets) -lrt

UNIXcrt.o: UNIXcrttemp.o
	gcc $(LNFLAGS) -o $@ UNIXcrttemp.o -lrt 

UNIXkbd.o: UNIXkbdtemp.o
	gcc $(LNFLAGS) -o $@ UNIXkbdtemp.o -lrt

startInitializeProcess.o: startInitializeProcess.c Struct.h
	$(CC) $(CFLAGS) startInitializeProcess.c

startMessageEnvelope.o: startMessageEnvelope.c Struct.h
	$(CC) $(CFLAGS) startMessageEnvelope.c

startSignalHandling.o: startSignalHandling.c startProcesses.h
	$(CC) $(CFLAGS) startSignalHandling.c

startForkAuxillary.o: startForkAuxillary.c Struct.h startProcesses.h
	$(CC) $(CFLAGS) startForkAuxillary.c

iProcesses.o: iProcesses.c publicProcesses.h Struct.h
	$(CC) $(CFLAGS) iProcesses.c

kPrimitives.o: kPrimitives.c Struct.h
	$(CC) $(CFLAGS) kPrimitives.c

UserProcesses.o: UserProcesses.c publicAPI.h
	$(CC) $(CFLAGS) UserProcesses.c

UserAPI.o: UserAPI.c publicProcesses.h
	$(CC) $(CFLAGS) UserAPI.c

Main.o: Main.c Struct.h publicProcesses.h startProcesses.h
	$(CC) $(CFLAGS) Main.c

UNIXcrttemp.o: UNIXcrt.c Struct.h
	$(CC) $(CFLAGS) -o $@ UNIXcrt.c

UNIXkbdtemp.o: UNIXkbd.c Struct.h
	$(CC) -c -wall -o $@ UNIXkbd.c
