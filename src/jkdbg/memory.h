/*
 * Usage:
 *   #include "jkdbg/memory.h"
 *
 * define ENAB_DEBUG during compilation, eg:
 *   gcc -W -Wall -g -O2 -DENAB_DEBUG -std=c89 -o foo foo.c
 */
#ifndef JKDBG_MEMORY_H
# define JKDBG_MEMORY_H

# include <stdio.h>
# include <stdlib.h>
# include <time.h>

# ifdef __cplusplus
extern "C" {
# endif

# ifdef ENAB_DEBUG

void *(*real_malloc)() = malloc;
void *(*real_calloc)() = calloc;
void *(*real_realloc)() = realloc;
void (*real_free)() = free;

void *jk_dbg_malloc();
void *jk_dbg_calloc();
void *jk_dbg_realloc();
void jk_dbg_free();

#  define JK_MLOG_FMT "[%c] [%9lx] %18s:%4lx. %8s(%8s) = %p,%lu\n"

#  define malloc(size) \
	jk_dbg_malloc(__FILE__, __LINE__, #size, (size))

#  define calloc(nmemb, size) \
	jk_dbg_calloc(__FILE__, __LINE__, #nmemb "," #size, (nmemb), (size))

#  define realloc(ptr, size) \
	jk_dbg_realloc(__FILE__, __LINE__, #ptr "," #size, (ptr), (size))

#  define free(ptr) \
	jk_dbg_free(__FILE__, __LINE__, #ptr, (void*)(ptr))

void *jk_dbg_malloc(filename, linenumber, variable, size)
	const char *filename, *variable;
	unsigned long linenumber;
	size_t size;
{
	time_t stamp = 0;
	void *ptr;

	time(&stamp);
	ptr = real_malloc(size);
	fprintf(stderr, JK_MLOG_FMT, 'A', stamp, filename, linenumber,
		"malloc", variable, ptr, size);

	return ptr;
}

void *jk_dbg_calloc(filename, linenumber, variables, nmemb, size)
	const char *filename, *variables;
	unsigned long linenumber;
	size_t nmemb, size;
{
	time_t stamp = 0;
	void *ptr;

	time(&stamp);
	ptr = real_calloc(nmemb, size);
	fprintf(stderr, JK_MLOG_FMT, 'A', stamp, filename, linenumber,
		"calloc", variables, ptr, size*nmemb);

	return ptr;
}

void *jk_dbg_realloc(filename, linenumber, variables, ptr, size)
	const char *filename, *variables;
	unsigned long linenumber;
	void *ptr;
	size_t size;
{
	time_t stamp = 0;
	void *new_ptr;

	time(&stamp);
	new_ptr = real_realloc(ptr, size);
	fprintf(stderr, JK_MLOG_FMT, 'A', stamp, filename, linenumber,
		"realloc", variables, new_ptr, size);

	if(new_ptr && (unsigned long)new_ptr != (unsigned long)ptr)
		fprintf(stderr, JK_MLOG_FMT, 'F', stamp, filename, linenumber,
			"free", "---", ptr, 0UL);

	return new_ptr;
}

void jk_dbg_free(filename, linenumber, variable, ptr)
	const char *filename, *variable;
	unsigned long linenumber;
	void *ptr;
{
	time_t stamp = 0;

	time(&stamp);
	fprintf(stderr, JK_MLOG_FMT, 'F', stamp, filename, linenumber,
		"free", variable, ptr, 0UL);
	real_free(ptr);
}

# endif /* ENAB_DEBUG */

# ifdef __cplusplus
}
# endif

#endif	/* !JKDBG_MEMORY_H */
