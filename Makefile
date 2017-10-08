SRC=main.c
OBJS=timer.o list.o 
EXE=capPortal

CFLAGS += -I$(OS_LIB_PATH)/include
LDFLAGS = -L$(OS_LIB_PATH)/
#LDFLAGS = -L$(OS_LIB_PATH)/ -los -lpthread -lrt

.PHONY:capPortal
all: capPortal

capPortal: $(OBJS)
	$(CC) $(SRC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(EXE) -pthread

timer.o: timer.c
	$(CC) $(CFLAGS) -c -o $@ $< -pthread

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(SRC:.c=.d) $(OBJS) $(EXE)
