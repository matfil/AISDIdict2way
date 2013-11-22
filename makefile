all : asd 

asd : asd.cc
	g++ asd.cc -g -o asd 
debug:
	g++ -g asd.cc -o asd_debug
	ddd asd_debug	
del :
	rm asd

