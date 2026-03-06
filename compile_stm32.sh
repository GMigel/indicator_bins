mkdir build_stm32
cd build_stm32

cmake .. \
 -DBUILD_STM32=ON \
 -DCMAKE_TOOLCHAIN_FILE=cmake/stm32_toolchain.cmake

cmake --build .