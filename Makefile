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

OBJS1=snmpdemoapp.o
OBJS2=example-daemon.o nstAgentSubagentObject.o
OBJS3=asyncapp.o
TARGETS=example-daemon snmpdemoapp asyncapp

CFLAGS=-I. `net-snmp-config --cflags`
BUILDLIBS=`net-snmp-config --libs`
BUILDAGENTLIBS=`net-snmp-config --agent-libs`

# shared library flags (assumes gcc)
DLFLAGS=-fPIC -shared

all: $(TARGETS)

example-daemon: $(OBJS2)
	$(CC) -lpq -o example-daemon $(OBJS2) $(BUILDAGENTLIBS) -lpq

clean:
	rm -f $(OBJS1) $(OBJS2) $(OBJS3) $(TARGETS)

nstAgentPluginObject.so: nstAgentPluginObject.c Makefile
	$(CC) $(CFLAGS) $(DLFLAGS) -c -o nstAgentPluginObject.o nstAgentPluginObject.c
	$(CC) $(CFLAGS) $(DLFLAGS) -o nstAgentPluginObject.so nstAgentPluginObject.o
