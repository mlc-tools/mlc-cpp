pushd .

cd ../
mkdir -p build_cli
cd build_cli
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .  -j8
cmake --install . --prefix "$HOME/.local"

popd
