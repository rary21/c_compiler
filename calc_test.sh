#!/bin/sh
gcc main.c
rm tmp.txt
./a.out calc_input.txt > tmp.txt
if diff tmp.txt calc_exp.txt > /dev/null; then
    echo calc_test success
else
    echo calc_test failed
fi