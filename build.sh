#!/bin/bash

g++ src/*.cpp -std=c++17 -I/usr/include/SDL2/ -I. -Iinclude/ -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -o bin/raycasting

#g++ -g -std=c++17 ./src/*.cpp -I./include -o ./bin/raycasting -lSDL2 -ldl
