#include "sys.h"


int SI_r;
unsigned int UI_r;

void check_bgt(int x)
{
	SI_r = 390;
	while (SI_r > x)
		SI_r--;

}

void check_bgtu(unsigned int x)
{
	UI_r = 390;
	while (UI_r > x)
		UI_r--;
}


void check_bltu(unsigned int x)
{
	UI_r = 0;
	while (UI_r < x)
		UI_r++;
}

void check_cmp_basic(void)
{
	volatile int x;
	volatile unsigned int y;

	x = 10;
	if (x > 10)
		err();
	if (x >= 11)
		err();
	if (x < 10)
		err();
	if (x <= 9)
		err();
	if (x == 0)
		err();
	if (x < 0)
		err();

	y = 10;
	if (y > 10)
		err();
	if (y >= 11)
		err();
	if (y < 10)
		err();
	if (y <= 9)
		err();
	if (y == 0)
		err();
	if ((int) y < 0)
		err();
}

void check_cmp(void)
{
	check_cmp_basic();
	check_bgt(333);
	if (SI_r != 333) err();
	check_bgtu(334);
	if (UI_r != 334) err();
	check_bltu(335);
	if (UI_r != 335) err();
}
