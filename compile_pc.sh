rm -rf build_pc
mkdir build_pc

#cd build_pc
#cmake .. -DBUILD_PC_SIM=ON
#cmake --build .

cmake -S . -B build_pc -DBUILD_PC_SIM=ON -DBUILD_STM32=OFF #-DPLATFORM_PC=1
cmake --build build_pc