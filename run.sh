#!/bin/bash

# Ejecuta el script de Python
python3 Processing/queryAudio.py

# Ejecuta el programa en C++
g++ -o main main.cpp
./main
