#! /usr/bin/make -f

##
# @file
# @brief

TARGETS=wait_msg
wait_msg_PGC_SRCS=wait_msg.pgc
wait_msg_SRCS=ini.c list.c
wait_msg_HEADS=ini.h list.h
wait_msg_CS=$(wait_msg_PGC_SRCS:.pgc=.c)
wait_msg_OBJS=$(wait_msg_CS:.c=.o) $(wait_msg_SRCS:.c=.o)
wait_msg_DEPS=$(wait_msg_OBJS:.o=.d)
CC=gcc-14
CFLAGS=-I/usr/include/postgresql -Wall -Wextra -Werror -std=c23 -Wpedantic -g
LDLIBS=-lecpg -lpq

.PHONY: all format clean

all: $(TARGETS)
$(TARGETS): $(wait_msg_OBJS)
%.o: %.c %.d
%.d: %.c
	$(CC) $(CFLAGS) -c -MM -o $@ $<
%.c: %.pgc
	ecpg $< -o $@
format:
	clang-format -i $(wait_msg_SRCS) $(wait_msg_PGC_SRCS) $(wait_msg_HEADS)
clean:
	$(RM) $(TARGETS) $(wait_msg_OBJS) $(wait_msg_DEPS) $(wait_msg_CS)
-include $(wait_msg_DEPS)
