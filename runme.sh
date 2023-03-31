#!/bin/bash
make
./input_file
./myprogram A B
gzip -k A B
gzip -k -cd B.gz | ./myprogram C
./block100 A D
rm -f result.txt
stat --printf 'file %n: %b block\n' A A.gz B B.gz C D > result.txt

