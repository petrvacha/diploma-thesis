#
# Projekt: Diploma thesis
# Autor:   Petr Vacha
# Datum:   2012
# 

CC=g++                              			# prekladac jazyka C++	
CFLAGS=-std=c++98 -Wall -pedantic-errors -g  	# parametry prekladace

cgp: cgp.cpp
	$(CC) $(CFLAGS) cgp.cpp -o cgp
