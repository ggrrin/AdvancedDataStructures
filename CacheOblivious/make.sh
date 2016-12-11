b=bin

mkdir $b 2> /dev/null

gcc -Ofast -std=c11 -o $b/sim.out sim/cachesim.c

#g++ -Wno-write-strings -O0 -g  -std=c++14 -o $b/main.out *.cpp
g++ -Wno-write-strings -Ofast -std=c++14 -o $b/main.out *.cpp

#g++ -Wno-write-strings -D NAIVE -O0 -g  -std=c++14 -o $b/main_naive.out *.cpp
g++ -Wno-write-strings -D SIM -Ofast -std=c++14 -o $b/main_sim.out *.cpp
