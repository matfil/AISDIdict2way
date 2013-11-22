all : asd 

asd : asd.cc
	g++ asd.cc -g -o asd 
debug:
	g++ -g asd.cc -o asd_debug
	ddd asd_debug
list: asd2wl.cpp
	g++-4.7 -std=c++11 -g -Wall -o list
del :
	rm asd
	rm list

