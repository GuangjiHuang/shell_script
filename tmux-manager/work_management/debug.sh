if [ ! -e debug ]; then
    mkdir debug
fi
cd debug 
[ $? -eq 0 ]  && rm -rf *
cmake -DCMAKE_BUILD_TYPE=Debug ../ && make && flag=1
