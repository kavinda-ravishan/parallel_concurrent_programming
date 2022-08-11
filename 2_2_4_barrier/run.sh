# !/bin/bash

# c++ -I /usr/local/include/boost main.cpp -o main -L /usr/local/lib -lboost_thread -lboost_system
make
LD_LIBRARY_PATH="/usr/local/lib" ./main