#!/bin/bash

extension=.cpt

x="$1"
bn=`basename "$x" .wav | tr " " "_"`
tfile=`mktemp /tmp/XXXXXX`
tdir=$tfile-tmp
echo $x $bn
mkdir $tdir
sox "$x" -r 6800 -c 1 -t dat -  | tail -n+3 | ./bin/demod | sox -t dat -r 3400 - -r 19200 -t dat - sinc -750 -n 16000 | tail -n+3 | ./bin/uart_deglitch | ./bin/uart_uart_split $tdir/ | tee $tdir/demod_info.txt

#Remove Glitches
for f in $tdir/*$extension
do
	size=`stat -c %s $f`
	echo $f $size
	if [ "$size" -lt 100 ]
	then
		echo "Deleting $f because it is to small" >> $tdir/demod_info.txt
		rm $f 
	fi
done

zip -j $bn $tdir/*
rm -r $tdir
rm $tfile
