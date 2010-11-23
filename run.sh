#!/bin/bash

echo Start Removing... 
rm ./primitiveRTX.o ./UNIXcrt.o ./UNIXkbd.o ./dummyKeyboardFile ./dummyCRTFile
echo ...Done Removing.

echo Start Compiling...
gcc -Wall -W ./Main.c ./startInitializeProcess.c ./startMessageEnvelope.c ./startSignalHandling.c ./startForkAuxillary.c ./iProcesses.c ./kPrimitives.c ./UserAPI.c ./UserProcesses.c -o primitiveRTX.o
gcc -Wall -W ./UNIXcrt.c -o UNIXcrt.o
gcc -Wall -W ./UNIXkbd.c -o UNIXkbd.o
echo ...Done Compiling

echo Start Executing...
./primitiveRTX.o
echo ...Done Executing!

