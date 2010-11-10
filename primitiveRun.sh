#!/bin/bash

echo Start Removing... 
rm ./primitiveRTX.o
echo ...Done Removing.

echo Start Compiling...
gcc -Wall -W ./Main.c ./startInitializeProcess.c -o primitiveRTX.o
echo ...Done Compiling

echo Start Executing...
./primitiveRTX.o
echo ...Done Executing!

