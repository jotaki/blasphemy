/*
 * code is ugly, should be cleaned up...
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

int64_t * prime_factors(int64_t n, size_t * size)
{
	int64_t * tmp, i,
		* result = NULL;

	if(n == 0)
		return NULL;

	*size = 0;
	if((n & 1) == 0) { // even (prime 2 is a factor)
		result = malloc(sizeof(int64_t));
		if(result == NULL)
			return NULL;

		result[(*size)++] = 2;
		n >>= __builtin_ctz(n);	/* divide by two until odd */
	}

	for(i = 3; n > 1; i += 2) {
		if((n % i))
			continue;

		tmp = realloc(result, ++(*size) * sizeof(int64_t));
		if(tmp == NULL) {
			if(result)
				free(result);

			return NULL;
		}
		result = tmp;
		result[(*size)-1] = i;

		for(; n % i == 0; n /= i)
			/* do nothing */ ;
	}

	return result;
}

int64_t * common_factors(int64_t n, size_t * size)
{
	int64_t * tmp, max, i, * result = NULL;

	if(n == 0)
		return NULL;
	
	*size = 0;
	max = sqrtl(n) + (n == 2? 0: 1);
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

void usage(const char * const arg0, int code)
{
	printf("Usage: %s [options] <number> [number...]\n", arg0);
	printf("%s version 0.3, available options:\n\n", arg0);
	printf("    -p    - Print only prime factors\n");

	exit(code);
}

int main(int argc, char *argv[])
{
	int i, pfonly = 0;
	size_t size, j;
	int64_t * f, n;


	if(argc < 2) { 
		usage(argv[0], 1);
	}

	/* XXX: HACK! XXX */
	if(argv[1][0] == '-' && argv[1][1] == 'p') {
		++pfonly; ++argv; --argc;
	}

	if(argc < 2) {
		usage(argv[-1], 1);
	}

	for(i = 1; i < argc; ++i) {
		n = atoll(argv[i]);
		if(pfonly == 0) {
			f = common_factors(n, &size);
			if(f == NULL)
				continue;

			printf("%" PRIu64 " is %sprime, with factors = { ",
					n, size == 2 && n != 1? "": "not ");

			for(j = 0; j  < size; j+=2)
				printf("%" PRIu64 "*%" PRIu64 ", ", f[j],
						f[j+1]);

			printf("\b\b }\n");
			free(f);
		} else {
			f = prime_factors(n, &size);
			if(f == NULL)
				continue;

			printf("%" PRIu64 " has prime factors: ", n);
			for(j = 0; j < size; ++j)
				printf("%" PRIu64 ", ", f[j]);

			printf("\b\b\n");
			free(f);
		}
	}

	return 0;
}
