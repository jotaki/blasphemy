all: xor

xor: xor.c
	gcc -o xor xor.c -W -Wall -s -O2

local-install:
	mkdir -p $(HOME)/bin
	install -m 0755 xor $(HOME)/bin/
	
clean:
	rm -f xor
