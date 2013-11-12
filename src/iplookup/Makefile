all: iplookup

iplookup:
	gcc -o iplookup iplookup.c -W -Wall -s -O2

local-install:
	mkdir -p $(HOME)/bin
	install -m 0755 iplookup $(HOME)/bin

clean:
	rm -f iplookup
