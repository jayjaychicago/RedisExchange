export CC=/home/dbdavidson/install/llvm31/bin/clang
export CXX=/home/dbdavidson/install/llvm31/bin/clang++
cmake -DCMAKE_BUILD_TYPE=Release -B../cmake_build/release -H.
cmake -DCMAKE_BUILD_TYPE=Debug -B../cmake_build/debug -H.
