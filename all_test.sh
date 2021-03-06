#!/bin/bash
mkdir -p build_clang
mkdir -p build_gcc-8
mkdir -p build_gcc-7


pushd build_gcc-8
rm -r *
export CC="gcc-8"
export CXX="g++-8"
cmake ..
if [ "$?" -ne "0" ] ; then
    echo "CMake failed..."
    popd
    exit -1
  fi
make
if [ "$?" -ne "0" ] ; then
    cd "$START_DIR"
    echo "Make failed..."
    popd
    exit -2
  fi
make test
if [ "$?" -ne "0" ] ; then
    cd "$START_DIR"
    echo "Test failed..."
    popd
    exit -3
  fi
popd

pushd build_gcc-7
rm -r *
export CC="gcc-7"
export CXX="g++-7"
cmake ..
if [ "$?" -ne "0" ] ; then
    cd "$START_DIR"
    echo "CMake failed..."
    popd
    exit -1
  fi
make
if [ "$?" -ne "0" ] ; then
    cd "$START_DIR"
    echo "Make failed..."
    popd
    exit -2
  fi
make test
if [ "$?" -ne "0" ] ; then
    cd "$START_DIR"
    echo "Test failed..."
    popd
    exit -3
  fi
popd

pushd build_clang
rm -r *
export CC="clang"
export CXX="clang++"
cmake ..
if [ "$?" -ne "0" ] ; then
    echo "CMake failed..."
    popd
    exit -1
  fi
make
if [ "$?" -ne "0" ] ; then
    cd "$START_DIR"
    echo "Make failed..."
    popd
    exit -2
  fi
make test
if [ "$?" -ne "0" ] ; then
    cd "$START_DIR"
    echo "Test failed..."
    popd
    exit -3
  fi
popd

unset CC
unset CXX
exit 0

