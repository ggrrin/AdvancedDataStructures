mkdir bin 2> /dev/null

g++ -Ofast -o bin/gen.out gen/hw3_heapgen.cpp

#g++ -Wno-write-strings -O0 -g  -std=c++14 -o bin/main.out *.cpp
g++ -Wno-write-strings -Ofast -std=c++14 -o bin/main.out *.cpp

#g++ -Wno-write-strings -D NAIVE -O0 -g  -std=c++14 -o bin/main_naive.out *.cpp
g++ -Wno-write-strings -D NAIVE -Ofast -std=c++14 -o bin/main_naive.out *.cpp
