#!/usr/bin/env bash

exec mlc -c tests/archero/configs  -d tests/archero/configs/data  --out_directory tests/archero/mg/ --out_data_directory tests/archero/ --auto_registration no --generate_intrusive no --generate_tests no --language cpp --side client --filter_code='!unit_tests/'