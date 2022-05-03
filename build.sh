rm -rf build/
mkdir build && cd build
cmake --log-level=VERBOSE -DCMAKE_CXX_COMPILER=/usr/bin/clang++ ..
make
./feditor
cd -
