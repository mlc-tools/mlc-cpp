set -e

pushd .

./build_and_install.sh

cd ../tests
mlc --config mlc.json

python3 test_cpp.py xml
python3 test_cpp.py json

python3 test_py.py xml
python3 test_py.py json

popd
