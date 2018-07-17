CFLAGS  = -g -std=gnu99 -Wall -Werror
CFLAGS += -I root/Summer/RedPitaya/api/include
LDFLAGS = -L root/Summer/RedPitaya/api/lib
LDLIBS = -lm -lpthread -lrp

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=)

all: $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) *.o
	$(RM) $(OBJS)
