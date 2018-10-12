#!/bin/sh
gcc main.c
gcc -o test_generation test_generation.c

./test_generation > calc_input.txt

./a.out calc_input.txt > result.txt
python calc_test.py calc_input.txt > calc_exp.txt

if diff result.txt calc_exp.txt > /dev/null; then
    echo calc_test success
else
    echo calc_test failed
fi