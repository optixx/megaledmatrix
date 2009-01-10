#!/bin/bash

#Converts a .pgm to a char array.


cat "$1" | {
    read d;
    read d;
    read d;
    echo "//Size: $d";
    read d;
    echo "unsigned char picture[] PROGMEM={";
    n=4;
    while read d; do
	echo -n "$d, ";
	let n=$n+1;
	if [ $n = 16 ]; then
	    n=0;
	    echo;
	fi
    done
    echo "};"
}