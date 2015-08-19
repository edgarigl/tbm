#if defined(__rpu__)
static const char *unitname = "rpu";
#elif defined(__apu__)
static const char *unitname = "apu";
#elif defined(__csu__)
static const char *unitname = "csu";
#elif defined(__pmu__)
static const char *unitname = "pmu";
#else
static const char *unitname = "unknown";
#endif

const char *get_unitname(void)
{
	return unitname;
}
