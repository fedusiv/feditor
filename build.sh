COMPILE=0
VERBOSE=0
RUN=0

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


while getopts cvr flag
do
    case ${flag} in
        c) COMPILE=1 ;;
        v) VERBOSE=1 ;;
        r) RUN=1 ;;
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
    cmake --log-level=VERBOSE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=/usr/bin/clang++ ..
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
