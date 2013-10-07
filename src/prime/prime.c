#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

int64_t * common_factors(int64_t n, size_t * size)
{
	int64_t * tmp, max, i, * result = NULL;

	if(n == 0)
		return NULL;
	
	*size = 0;
	max = sqrtl(n) + 1;
	for(i = 1; i <= max; ++i) {
		if((n % i) == 0) {
			*size += 2;
			tmp = realloc(result, (*size) * sizeof(int64_t));
			if(tmp == NULL) {
				if(result)
					free(result);
				fprintf(stderr, "Not enough memory\n");
				return NULL;
			}
			result = tmp;
			result[(*size)-2] = i;
			result[(*size)-1] = n/i;
		}
	}
	return result;
}

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
	size_t size, j;
	uint64_t * f, n;

	if(argc < 2) {
		printf("Usage: %s <number> [number...]\n", argv[0]);
		return 1;
	}

	for(i = 1; i < argc; ++i) {
		n = atoll(argv[i]);
		f = common_factors(n, &size);
		if(f == NULL)
			continue;

		printf("%" PRIu64 " is %sprime, with factors = { ",
				n, size == 2? "": "not ");

		for(j = 0; j  < size; j+=2)
			printf("%" PRIu64 "*%" PRIu64 ", ", f[j], f[j+1]);

		printf("\b\b }\n");
		free(f);
	}

	return 0;
}
