#!/bin/bash

echo Start Removing... 
rm ./primitiveRTX.o ./UNIXcrt.o ./UNIXkbd.o 
echo ...Done Removing.

echo Start Compiling...
gcc -Wall -W ./Main.c ./InitializeProcess.c ./MessageEnvelope.c ./SignalHandling.c ./ForkAuxillary.c ./iProcesses.c ./UserAPI.c ./kPrimitives.c -o primitiveRTX.o
gcc -Wall -W ./UNIXcrt.c -o UNIXcrt.o
gcc -Wall -W ./UNIXkbd.c -o UNIXkbd.o
echo ...Done Compiling

echo Start Executing...
./primitiveRTX.o
echo ...Done Executing!

