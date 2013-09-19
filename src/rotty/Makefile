all: rotty

rotty: rotty.c
	gcc -o rotty rotty.c -W -Wall -s -O2
	ln -svf rotty rot13
	ln -svf rotty rot47

local-install: rotty
	mkdir -p $(HOME)/bin
	install -m 0755 rotty $(HOME)/bin/
	ln -svf $(HOME)/bin/rotty $(HOME)/bin/rot13
	ln -svf $(HOME)/bin/rotty $(HOME)/bin/rot47

clean:
	rm -f rotty rot13 rot47
