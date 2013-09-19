#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "rotty.h"

int main(int argc, char *argv[])
{
	char *app = strchr(argv[0], '/');

	if(app)
		++app;
	else
		app = argv[0];

	if(strcmp(app, "rot13") == 0)
		return rot13_main(argc, argv);
	else if(strcmp(app, "rot47") == 0)
		return rot47_main(argc, argv);
	else if(strcmp(app, "rotty") == 0) {
		return rotty_main(argc, argv);
	}

	printf("Huh?\n");
	return 0;
}

char *rot13(char *buffer)
{
	char *p = buffer;

	while(*p) {
		if(isupper(*p)) *p = rotate(*p, 'A', 13, 26);
		else if(islower(*p)) *p = rotate(*p, 'a', 13, 26);

		++p;
	}

	return buffer;
}

char *rot5(char *buffer)
{
	char *p = buffer;

	while(*p) {
		if(isdigit(*p)) *p = rotate(*p, '0', 5, 10);
		++p;
	}

	return buffer;
}

int rot13_main(int argc, char *argv[])
{
	char buffer[BUFSIZ];

	UNUSED(argc);
	UNUSED(argv);

	while(fgets(buffer, BUFSIZ - 1, stdin)) {
		if(argc > 1) rot5(buffer);
		printf("%s", rot13(buffer));
	}

	return 0;
}

char *rot47(char *buffer)
{
	char *p = buffer;

	while(*p) {
		if(*p > 32) *p = rotate(*p, 33, 47, 94);
		++p;
	}

	return buffer;
}

int rot47_main(int argc, char *argv[])
{
	char buffer[BUFSIZ];

	UNUSED(argc);
	UNUSED(argv);

	while(fgets(buffer, BUFSIZ - 1, stdin))
		printf("%s", rot47(buffer));

	return 0;
}

int rotty_main(int argc, char *argv[])
{
	char buffer[BUFSIZ], *p;
	int rotation = 0;

	if(argc < 2) {
		printf("Usage: %s <rotation count>\n", argv[0]);
		return 1;
	}

	rotation = atoi(argv[1]);
	if(1 > rotation) {
		printf("Invalid rotation: %s\n", argv[1]);
		return 1;
	}

	while(fgets(buffer, BUFSIZ - 1, stdin)) {
		p = buffer;

		while(*p) {
			if(*p > 32 && isupper(*p))
				*p = rotate(*p, 'A', rotation, 26);
			else if(*p > 32 && islower(*p)) 
				*p = rotate(*p, 'a', rotation, 26);

			++p;
		}

		printf("%s\n", buffer);
	}

	return 0;
}
