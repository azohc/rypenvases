#! /bin/bash
#script para compilar y ejecutar el código
g++ -g -std=c++0x main.cpp -o main && ./main
rm main
rm main.exe