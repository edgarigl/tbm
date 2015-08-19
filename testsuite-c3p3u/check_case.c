#include "sys.h"

int mycase(int x)
{
	int r;

	r = 0;
	switch (x)
	{
		case 'a': r = 'u'; break;
		case 'b': r = 'p'; break;
		case 'c': r = '9'; break;
		case 'w': r = 'e'; break;
		case 'z': r = 'a'; break;
		case '1': r = 'a'; break;
		case '2': r = 'g'; break;
		case '3': r = 'u'; break;
		case '4': r = '7'; break;
	}
	return r;
}


void check_case(void)
{
	volatile int a;

	puts(__func__);

	a = mycase('a');
	if (a != 'u')
		err();
	a = mycase('b');
	if (a != 'p')
		err();
}
