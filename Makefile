#
# Warning: you may need more libraries than are included here on the
# build line.  The agent frequently needs various libraries in order
# to compile pieces of it, but is OS dependent and we can't list all
# the combinations here.  Instead, look at the libraries that were
# used when linking the snmpd master agent and copy those to this
# file.
#

CC=gcc -Wimplicit-function-declaration
#CC=gcc -Werror-implicit-function-declaration

# -Wno-implicit-function-declaration
# -Wl,--no-undefined

OBJS=daemon.o subagentObject.o
TARGETS=daemon

BUILDAGENTLIBS=`net-snmp-config --agent-libs`

# all: $(TARGETS)

daemon: $(OBJS)
	$(CC) -lpq -o daemon $(OBJS) $(BUILDAGENTLIBS) -lpq

clean:
	rm -f $(OBJS) $(TARGETS)