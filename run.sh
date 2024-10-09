folder_name=$(basename "$(pwd)")
executable="./cmake-build-debug/${folder_name}"

mpic++ -DPARALLEL -o "${executable}" main.cpp
mpiexec -np 4 "${executable}"
