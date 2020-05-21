#!/bin/bash

for T in bin/*; do 
		echo valgrind --leak-check=full $T
		valgrind --demangle=yes --log-file=memcheck.log --leak-check=full $T 
		grep "All heap blocks were freed" memcheck.log
		rm memcheck.log
	done
