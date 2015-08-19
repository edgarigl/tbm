typedef void (*initcallfn)(void);

initcallfn __initcalls_start;
initcallfn __initcalls_end;

void __initcalls_exec(void)
{
	initcallfn *fn = &__initcalls_start;
	while (fn < &__initcalls_end) {
		(*fn)();
		fn++;
	}
}
