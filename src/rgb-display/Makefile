all: rgb-display

rgb-display:
	gcc -o rgb-display rgb-display.c -W -Wall -s -O2 -lX11

local-install:
	mkdir -p $(HOME)/bin
	install -m 0755 rgb-display $(HOME)/bin

clean:
	rm -f rgb-display
