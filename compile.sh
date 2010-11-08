#!/bin/bash
echo Start Removing:
rm ./Main.o ./InitializeProcess.o ./MessageEnvelope.o ./SignalHandling.o ./ForkAuxillary.o ./UNIXcrt.o ./UNIXkbd.o ./iProcesses.o ./UserProcess.o ./kPrimitives.o
echo Start Compile:
gcc -Wall -W -c ./Main.c ./InitializeProcess.c ./MessageEnvelope.c ./SignalHandling.c ./ForkAuxillary.c ./iProcesses.c ./UserProcess.c ./kPrimitives.c
gcc -Wall -W -c ./UNIXcrt.c 
gcc -Wall -W -c ./UNIXkbd.c 

echo Done Compile!

