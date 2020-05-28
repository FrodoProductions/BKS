CFLAGS=-std=c11 -Wall -Wextra -pedantic -O2
DEFINES=
CPPFLAGS=-MMD -MF $@.d $(DEFINES)

programs=scheduler
obj-y=scheduler_wrapper.o scheduler.o
scheduler: $(obj-y)

all: $(programs)

-include *.d

# pattern rules
%: %.o
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

%.o: %.c
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<

%.d: ;

clean:
	@$(RM) -fv $(programs) $(obj-y) $(obj-y:=.d)

.SUFFIXES:
.PHONY: all clean
.PRECIOUS: $(obj-y) $(obj-y:=.d)
