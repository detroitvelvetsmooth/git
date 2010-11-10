#!/bin/bash
echo Start Removing..
rm ./Main.o ./startInitializeProcess.o ./startMessageEnvelope.o ./startSignalHandling.o ./startForkAuxillary.o ./iProcesses.o ./kPrimitives.o ./UserAPI.o ./UserProcesses.o 
rm  ./UNIXcrt.o 
rm  ./UNIXkbd.o 
echo ...Done Removing

echo Start Compile...
gcc -Wall -W -c ./Main.c ./startInitializeProcess.c ./startMessageEnvelope.c ./startSignalHandling.c ./startForkAuxillary.c ./iProcesses.c ./kPrimitives.c ./UserAPI.c ./UserProcesses.c 
gcc -Wall -W -c ./UNIXcrt.c 
gcc -Wall -W -c ./UNIXkbd.c 

echo ...Done Compile!


