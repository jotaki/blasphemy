all: xidle

xidle:
	gcc -o xidle xidle.c -lX11 -lXext -lXss -DPROG_PATH=$(HOME)/bin/xidle

local-install:
	mkdir -p $(HOME)/bin
	install -m 0755 xidle $(HOME)/bin/

clean:
	rm -f xidle
