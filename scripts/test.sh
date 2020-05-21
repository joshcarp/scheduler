#!/bin/bash

for T in bin/*_test.o; do 
		echo $T
		out=$(./$T)
		if [ $? -ne 0 ]; then
			echo Address sanitising failed
			exit 1
		elif echo $out | grep FAIL; then
			exit 1
		else 
			echo $out
		fi
done