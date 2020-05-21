#!/bin/bash

for T in bin/*_test.o; do 
		echo $T
		./$T
	done