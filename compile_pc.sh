rm -rf build_pc
mkdir build_pc
cd build_pc

cmake .. -DBUILD_PC_SIM=ON
cmake --build .