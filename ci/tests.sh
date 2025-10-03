pushd .

./build_and_install.sh

cd ../tests/simple_test
mlc --config mlc.json

python3 test_cpp.py xml
python3 test_cpp.py json

python3 test_py.py xml
rm -r generated_py/xml
python3 test_py.py json

popd
