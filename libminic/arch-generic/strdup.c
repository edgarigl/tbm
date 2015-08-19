/*
 * Copyright 2009 (C) Edgar E. Iglesias <edgar.iglesias@gmail.com>
 */
#include <stdlib.h>
#include <string.h>

char *strdup(const char *s)
{
	size_t len = strlen(s);
	char *n;

	n = malloc(len + 1);
	return n ? memcpy(n, s, len + 1) : NULL;
}
