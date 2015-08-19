void *memcpy(void *d, const void *s, int n)
{
	char l;
	unsigned char *bd;
	const unsigned char *bs;

	bd = d;
	bs = s;
	while (n--) {
		l = *bs++;
		*bd++ = l;
	}
	return d;
}
