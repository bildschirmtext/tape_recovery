#!/bin/bash

extension=.cpt

for x in "$1"
do
	bn=`basename "$x" .wav | tr " " "_"`
	echo $x $bn
	mkdir $bn
	rm $bn/*
	sox "$x" -r 6800 -c 1 -t dat -  | tail -n+3 | ./bin/demod | sox -t dat -r 3400 - -r 19200 -t dat - | tail -n+3 | ./bin/uart_deglitch | ./bin/uart_uart_split $bn/ | tee $bn/demod_info.txt
	zip -r $bn $bn/
done
