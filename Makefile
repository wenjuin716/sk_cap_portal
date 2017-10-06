SRC=main.c
OBJS=$(SRC:.c=.o) 
EXE=capPortal

CFLAGS += -I$(OS_LIB_PATH)/include
LDFLAGS = -L$(OS_LIB_PATH)/
#LDFLAGS = -L$(OS_LIB_PATH)/ -los -lpthread -lrt

.PHONY:capPortal
all: capPortal

capPortal: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(EXE)

#dhcpd: $(DHCPD_OBJS) 
#	$(CC) $(DHCPD_OBJS) $(CFLAGS) $(LDFLAGS) -o dhcpd  && $(STRIP) dhcpd && ls -l dhcpd

#dhcpc: $(DHCPC_OBJS) 
#	$(CC) $(DHCPC_OBJS) $(CFLAGS) $(LDFLAGS) -o dhcpc  && $(STRIP) dhcpc && ls -l dhcpc
	
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(SRC:.c=.d) $(OBJS) $(EXE)
