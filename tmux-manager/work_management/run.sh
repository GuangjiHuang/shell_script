#! /bin/bash
exe_name=main
#build_type=$1
build_type="Release"
#if [[ $build_type == "Debug" ]] || [[ $build_type == "Release" ]]; then  # this is the match
#if [ $build_type = "Debug" -o $build_type = "Release" ]; then
#if [ "$build_type" = "Debug" -o "$build_type" = "Release" ]; then
if [[ $build_type =~ ([Dd]ebug|[Rr]elease) ]];then
    exe_name="${exe_name}_${build_type}"
fi
echo $exe_name
echo "ok"
#
if [ -f $exe_name ]; then
    rm $exe_name
fi
if [ ! -e build ]; then
    mkdir build
fi
cd build 
[ $? -eq 0 ]  && rm -rf *
cmake -DUSE_COLOR=ON -DCMAKE_BUILD_TYPE=$build_type ../ && make -j20 && flag=1
cd ..
if [ $flag -eq 1 ]; then
    #clear
    if [ "$exe_name" != "main.exe" ]; then
        mv $exe_name "main.exe"
    fi
    #./$main.exe  "data"
fi
