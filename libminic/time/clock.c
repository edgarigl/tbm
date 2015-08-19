#include <time.h>
#ifdef __C3P3U__
#include <include/arch/time.h>
#endif

clock_t clock(void)
{
#ifdef __C3P3U__
	return timer_uptime();
#else
	static clock_t clk = 0;
	return clk++;
#endif
}
