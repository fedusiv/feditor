#!/bin/sh
COMPILE=0
VERBOSE=0
RUN=0
DEBUG=0

make_func()
{
    # verbose mode for make
    if [ ${VERBOSE} -eq 1 ];
    then
        make VERBOSE=1
    else
        make
    fi
}

cmake_func()
{
    if [ ${DEBUG} -eq 1 ];
    then
        echo "Debug build"
        cmake --log-level=VERBOSE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=/usr/bin/clang++ ..
    else
        echo "Release build"
        cmake --log-level=VERBOSE -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=/usr/bin/clang++ ..
    fi
}

# -c compile, only to run make
# -v verbose make
# -e execute program after building
# -d debug settings for cmake
while getopts cved flag
do
    case ${flag} in
        c) COMPILE=1 ;;
        v) VERBOSE=1 ;;
        e) RUN=1 ;;
        d) DEBUG=1 ;;
    esac
done

if [ ${COMPILE} -eq 1 ];
then
    echo "Running only compilation"
    cd build
    make_func
else
    echo "Rebulding.."
    rm -rf build/
    mkdir build && cd build
    cmake_func
    make_func
    cp -r ../assets .
fi

# run only if needed
if [ ${RUN} -eq 1 ]
then
    echo "Run"
    ./feditor
fi
#cd -
