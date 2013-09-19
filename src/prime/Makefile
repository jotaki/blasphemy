all: prime

prime: prime.c
	gcc -o prime prime.c -W -Wall -s -O2 -lm

local-install: prime
	mkdir -p $(HOME)/bin
	install -m 0755 prime $(HOME)/bin/
clean:
	rm -f prime
