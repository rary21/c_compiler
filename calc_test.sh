#!/bin/sh
TEST_DIR="./test_code/"
make > /dev/null

pushd ${TEST_DIR} > /dev/null
gcc -o test_generation test_generation.c

./test_generation > calc_input.txt

../mycc calc_input.txt > result.txt
python calc_test.py calc_input.txt > calc_exp.txt

if diff result.txt calc_exp.txt > /dev/null; then
    echo calc_test success
else
    echo calc_test failed
fi

popd > /dev/null