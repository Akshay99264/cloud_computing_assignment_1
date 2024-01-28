#!/bin/bash
make
gcc -w -o test test.c 
sudo insmod solution_2_I.ko
echo -e "\n\n\nOutput\n"
sudo ./test
sudo rmmod solution_2_I.ko
echo -e "\n"
make clean
rm test
