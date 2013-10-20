#! /bin/bash

# This script will build and run the huffman programm

echo `make`

if [ "$1" = "--debug" ] || [ "$1" = "-d" ];
  then
    /usr/bin/gdb huffman
elif [ "$1" = "--help" ] || [ "$1" = "-h" ];
  then
    echo "Help text!"
    echo `huffman -h`
else
  echo `time huffman $1 > huffman.log`
cat huffman.log
fi 
