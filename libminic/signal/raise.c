#include <stdlib.h>
#include <unistd.h>

int raise(int sig)
{
	exit (1);
}

