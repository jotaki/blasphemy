#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char *key, buf[512], tmp;
	int i, len, ki, klen, r = 0;

	if(argc < 2) {
		printf("Usage: %s <key>\n", argv[0]);
		return 1;
	}
	key = argv[1];
	klen = strlen(key);
	ki = 0;

	while(fgets(buf, sizeof(buf)-1, stdin)) {
		len = strlen(buf);
		for(i = 0; i < len; ++i, ++ki, r=(~r & ki) << 1) {
			tmp = buf[i]^((key[ki%klen] + r) & 0xff);
			if(!tmp) tmp = buf[i];
			printf("%c", tmp);
			fflush(stdout);
		}
	}
	return 0;
}
