#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int is_prime(unsigned long long number)
{
        unsigned long long j,  n;

        /* not prime conditions */
        if(number < 2)
                return 0;
        else if(number == 2)
                return 1;
        else if((number & 1) == 0)
                return 0;

        n = sqrtl(number) + 1;
        for(j = 3; j < n; j += 2)
                if((number % j) == 0)
                        return 0;

        return 1;
}


int main(int argc, char *argv[])
{
	int i;

	if(argc < 2) {
		printf("Usage: %s <number> [number...]\n", argv[0]);
		return 1;
	}

	for(i = 1; i < argc; ++i) {
		printf("%s is %sprime\n", argv[i],
			is_prime(atoll(argv[i]))? "": "not ");
	}

	return 0;
}
