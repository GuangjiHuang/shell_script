#! /bin/bash

cmake_test_dir=./test_build
if [ ! -d $cmake_test_dir ];then
    mkdir $cmake_test_dir
fi

cd $cmake_test_dir
if [ $? -eq 0 ];then
    rm -rf *
fi

clear
cmake -DCMAKE_BUILD_TYPE=Release ..

cd ..

