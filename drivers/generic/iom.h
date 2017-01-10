#include "reg-iom.h"

void iom_pit_start(void *base, unsigned int pit, unsigned int count, bool oneshot);
void iom_pit_stop(void *base, unsigned int pit);
int iom_putchar(void *base, int c);
int iom_getchar(void *base);
int __iom_getchar(void *base);
