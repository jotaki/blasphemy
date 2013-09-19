#ifndef ROTTY_H
# define ROTTY_H Sup?

int rot13_main(int argc, char *argv[]);
int rot47_main(int argc, char *argv[]);
int rotty_main(int argc, char *argv[]);

char *rot13(char *buffer);
char *rot5(char *buffer);
char *rot47(char *buffer);

inline int rotate(int input, int base, int y, int x)
{
	return ( ((input - base) + y) % x) + base;
}

# ifndef UNUSED
#  define UNUSED(x) x=x
# endif

#endif /* !ROTTY_H */
