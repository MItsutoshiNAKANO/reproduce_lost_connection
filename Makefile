#! /usr/bin/make -f

##
# @file
# @brief Makefile for the wait_msg and send_msg programs
# @details
#   This Makefile is used to compile the wait_msg and send_msg programs.
#   It uses the ECPG preprocessor to generate C code from the .pgc files.
#   It also uses clang-format to format the code and includes rules for
#   cleaning up the generated files.
# @note
#   The Makefile assumes that the ECPG preprocessor is installed and available
#   in the PATH.
#   It also assumes that the PostgreSQL development headers are installed and
#   available in /usr/include/postgresql.
# @note
#   The Makefile uses gcc-14 as the compiler and includes flags for warnings,
#   errors, and debugging.
#   It also includes flags for the ECPG and PostgreSQL libraries.
# @note
#   The Makefile includes a rule for generating dependency files for the object
#   files.
#   This allows for automatic rebuilding of the object files when the source
#   files change.
#   The dependency files are included at the end of the Makefile.

TARGETS=wait_msg send_msg
HEADS=ini.h list.h
wait_msg_PGC_SRCS=wait_msg.pgc
wait_msg_SRCS=ini.c list.c
wait_msg_CS=$(wait_msg_PGC_SRCS:.pgc=.c)
wait_msg_OBJS=$(wait_msg_CS:.c=.o) $(wait_msg_SRCS:.c=.o)
wait_msg_DEPS=$(wait_msg_OBJS:.o=.d)
wait_msg_CFLAGS=-I/usr/include/postgresql
wait_msg_LDLIBS=-lecpg -lpq

send_msg_SRCS=send_msg.c ini.c list.c
send_msg_OBJS=$(send_msg_SRCS:.c=.o)
send_msg_DEPS=$(send_msg_OBJS:.o=.d)

CC=gcc-14
CFLAGS=-Wall -Wextra -Werror -std=c23 -Wpedantic -g

.PHONY: all format clean

all: $(TARGETS)

wait_msg: CFLAGS+=$(wait_msg_CFLAGS)
wait_msg: LDLIBS=$(wait_msg_LDLIBS)
wait_msg: $(wait_msg_OBJS)
send_msg: $(send_msg_OBJS)
%.o: %.c %.d
%.d: %.c
	$(CC) $(CFLAGS) -c -MM -o $@ $<
%.c: %.pgc
	ecpg $< -o $@
format:
	clang-format -i $(wait_msg_SRCS) $(wait_msg_PGC_SRCS) $(send_msg_SRCS)\
		$(HEADS)
clean:
	$(RM) $(TARGETS) $(wait_msg_OBJS) $(wait_msg_DEPS) $(wait_msg_CS)\
		$(send_msg_OBJS) $(send_msg_DEPS)
-include $(wait_msg_DEPS) $(send_msg_DEPS)
