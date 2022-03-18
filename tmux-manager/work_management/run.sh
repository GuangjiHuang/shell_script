#! /bin/bash
if [ -f ./main ]; then
    rm main
fi
if [ ! -e build ]; then
    mkdir build
fi
cd build 
[ $? -eq 0 ]  && rm -rf *
cmake ../ && make && flag=1
cd ..
if [ $flag -eq 1 ]; then
    clear
    ./main "data"
fi
