#!/bin/bash

extension=.cpt

for x in "$1"
do
	bn=`basename "$x" .wav | tr " " "_"`
	echo $x $bn
	mkdir $bn
	cd $bn
	sox ../"$x" -r 6800 -c 1 -t dat - sinc -n 1024 200-3500 | tail -n+3 | ../bin/demod | ../bin/split
	for y in *.dat
	do
		bn2=`basename "$y" .dat`
		sox -t dat -r 3400 "$y" -r 19200 -t dat - | tail -n+3 | ../bin/uart_deglitch | ../bin/uart_uart > $bn2.$extension
		rm $y
		ls -l $bn2.$extension
	done
	cd ..
	zip -r $bn $bn/*.$extension
done
