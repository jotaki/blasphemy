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

#  define JK_MLOG_FMT "[%c] [%11x] %16s:%lu %8s(%12s) = %p,%ld\n"


#  define malloc(size) \
	jk_dbg_malloc(__FILE__, __LINE__, ##size, (size))

#  define calloc(nmemb, size) \
	jk_dbg_calloc(__FILE__, __LINE__, ##nmemb "," ##size, (nmemb), (size))

#  define realloc(ptr, size) \
	jk_dbg_realloc(__FILE__, __LINE__, ##ptr "," ##size, (ptr), (size))

#  define free(ptr) \
	jk_dbg_free(__FILE__, __LINE__, ##ptr, (void*)(ptr))

inline void *jk_dbg_malloc(filename, linenumber, variable, size)
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

inline void *jk_dbg_calloc(filename, linenumber, variables, nmemb, size)
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

inline void *jk_dbg_calloc(filename, linenumber, variables, ptr, size)
	const char *filename, *variables;
	unsigned long linenumber;
	void *ptr;
	size_t size
{
	time_t stamp = 0;
	void *new_ptr;

	time(&stamp);
	new_ptr = real_realloc(ptr, size);
	fprintf(stderr, JK_MLOG_FMT, 'A', stamp, filename, linenumber,
		"realloc", variables, new_ptr, size);

	if((unsigned long)new_ptr != (unsigned long)ptr)
		fprintf(stderr, JK_MLOG_FMT, 'F', stamp, filename, linenumber,
			"free", "---", ptr, 0L);

	return new_ptr;
}

inline void jk_dbg_free(filename, linenumber, variable, ptr)
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
