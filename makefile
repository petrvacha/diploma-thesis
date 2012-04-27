#
# Projekt: Diploma thesis
# Autor:   Petr Vacha
# Datum:   2012
# 

CC=g++                              # překladač jazyka C
CFLAGS=-std=c++98 -Wall -pedantic-errors -g  # parametry překladače

cgp: cgp.cpp
	$(CC) $(CFLAGS) cgp.cpp -o cgp
