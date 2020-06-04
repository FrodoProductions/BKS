CFLAGS=-std=c11 -Wall -Wextra -pedantic -O2
CPPFLAGS=-MMD -MF $*.d
# Needs libreadline-dev on Debian/Ubuntu
LDLIBS=-lreadline

program=memory_Scheller_Brandenburg
obj-y=$(program).o memory_wrapper.o

all: $(program)
$(program): $(obj-y)

-include *.d

# pattern rules
%: %.o
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

%.o: %.c
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<

%.d: ;

clean:
	@$(RM) -fv $(program) $(obj-y) $(obj-y:.o=.d)

.SUFFIXES:
.PHONY: all clean
.PRECIOUS: $(obj-y)
