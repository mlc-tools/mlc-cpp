set -e

pushd .

# ./build_and_install.sh

function build(){
	pushd . > /dev/null
	cd ../
	mkdir -p build_cli
	cd build_cli
	cmake -DCMAKE_BUILD_TYPE=Release ..
	cmake --build .  -j8
	cmake --install . --prefix "$HOME/.local"
	popd > /dev/null
}

build

cd ../tests
~/.local/bin/mlc --config mlc.json
python3 test_partial_classes.py ~/.local/bin/mlc

python3 test_cpp.py xml
python3 test_cpp.py json

python3 test_py.py xml
python3 test_py.py json

popd
