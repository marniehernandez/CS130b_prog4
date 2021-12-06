# CC=gcc
# CFLAGS=-I.
CXX=clang++

prog4: prog4.o
	${CXX} prog4.o -o prog4

prog4.o: prog4.cpp
	${CXX} -c prog4.cpp -o prog4.o