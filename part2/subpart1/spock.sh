#!/bin/bash
make
gcc -w -o test test.c 
sudo insmod changePhysical.ko
echo -e "\n\n\nOutput\n"
sudo ./test
sudo rmmod changePhysical.ko
echo -e "\n"
make clean
