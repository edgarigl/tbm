
CFLAGS   += -Wall -W -O2 -g -D__cris__
CPPFLAGS += -I ../include

TESTCASES  = check-malloc
TESTCASES += check-memcmp
TESTCASES += check-ctype
TESTCASES += check-strlen
TESTCASES += check-memmove

all: $(TESTCASES)

PHONY: check
check: all
	for c in $(TESTCASES); do ./$$c; done

clean:
	$(RM) $(TESTCASES)

