#makefile

CC = g++ -std=c++98 -Wall -pedantic -g -O2 -Wextra -Werror

huffman: huffman.o 
	$(CC) -o huffman huffman.o
huffman.o: huffman.cpp huffman.hpp
	$(CC) -c huffman.cpp
