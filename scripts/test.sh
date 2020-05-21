#!/bin/bash

for T in bin/*_test.o; do 
		echo $T
		out=$(./$T)
		echo $out
		if echo $out | grep FAIL; then
			exit 1
		elif [ $? -ne 0 ]; then
			echo Address sanitising failed
			exit 1
		fi
done