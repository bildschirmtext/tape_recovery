#!/bin/bash

for x in *.c
do
	bn=`basename $x .c`
	gcc $x -o $bn
done
