typedef void (*testcallfn)(void);

testcallfn __testcalls_start;
testcallfn __testcalls_end;

int __testcalls_exec(void)
{
	testcallfn *fn = &__testcalls_start;
	while (fn < &__testcalls_end) {
		(*fn)();
		fn++;
	}
	return fn - &__testcalls_start;
}
