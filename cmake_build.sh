set -x
cd build
BASEPATH=..
BUILDTYPE=Debug


cmake ${BASEPATH}
make VERBOSE=1

rm -rf CMakeCache.txt
rm -rf CMakeFiles
rm -rf cmake_install.cmake
rm -rf Makefile
rm -rf CTestTestfile.cmake

set +x
