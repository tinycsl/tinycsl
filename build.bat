mkdir build-win
cd build-win
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
@REM cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
@REM mingw32-make
@REM cmake --build . -- VERBOSE=1
cd ..