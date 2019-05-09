#! /bin/bash

#script para compilar y ejecutar el código

sencillas=1
elaboradas=2

g++ -g -std=c++0x main.cpp -o main 

# descomentar este bloque para ejecutar sin entrada-salida
# primer argumento = N, segundo argumento = tipo de cotas 
./main 12 $sencillas
./main 12 $elaboradas
./main 18 $sencillas
./main 18 $elaboradas
./main 24 $sencillas
./main 24 $elaboradas


# descomentar esta linea para ejecutar con entrada-salida
# ./main

rm main