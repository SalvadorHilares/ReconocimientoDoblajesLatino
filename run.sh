#!/bin/bash

# Ejecuta el script de Python
python3 Processing/queryAudio_test.py

# Ejecuta el programa en C++
g++ -o main_test main_test.cpp
./main_test
