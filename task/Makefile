# --- Boilerplate things for every makefile ---

SHELL=/bin/sh
CC=cc
CFLAGS=-Wall -Wextra -Werror

# --- Output locations ---

# folder for final client and server binaries
TARGET_FOLDER=.
# folder for all interim compiled stuff
BINARY_FOLDER=bin

# --- Main targets ---

SERVER_NAME=server
CLIENT_NAME=client
NAME=minitalk

SERVER_NAME_IN_FOLDER=${TARGET_FOLDER}/${SERVER_NAME}
CLIENT_NAME_IN_FOLDER=${TARGET_FOLDER}/${CLIENT_NAME}

${NAME}: ${SERVER_NAME_IN_FOLDER} ${CLIENT_NAME_IN_FOLDER}
all: ${NAME}

# --- Objects compilation --

SRCS = \
bitstring_a.c bitstring_b.c \
client.c \
common.c \
memory.c \
parse_client_args.c \
print.c \
server_message.c \
server.c \
signed_strlen.c

OBJS=${addprefix ${BINARY_FOLDER}/,${SRCS:.c=.o}}

${BINARY_FOLDER}/bitstring_a.o: bitstring_a.c bitstring.h memory.h print.h
${BINARY_FOLDER}/bitstring_b.o: bitstring_b.c bitstring.h
${BINARY_FOLDER}/client.o: client.c common.h parse_client_args.h print.h
${BINARY_FOLDER}/common.o: common.c common.h
${BINARY_FOLDER}/memory.o: memory.c memory.h
${BINARY_FOLDER}/parse_client_args.o: parse_client_args.c common.h parse_client_args.h print.h signed_strlen.h
${BINARY_FOLDER}/print.o: print.c print.h signed_strlen.h
${BINARY_FOLDER}/server_message.o: server_message.c server_message.h
${BINARY_FOLDER}/server.o: server.c bitstring.h common.h print.h server_message.h
${BINARY_FOLDER}/signed_strlen.o: signed_strlen.h

${OBJS}: ${BINARY_FOLDER}/%.o: %.c
	mkdir -p $(@D)
	${CC} ${CFLAGS} $< -o $@ -c

# --- Binaries linking ---

${SERVER_NAME_IN_FOLDER}: \
${BINARY_FOLDER}/bitstring_a.o \
${BINARY_FOLDER}/bitstring_b.o \
${BINARY_FOLDER}/common.o \
${BINARY_FOLDER}/memory.o \
${BINARY_FOLDER}/print.o \
${BINARY_FOLDER}/server_message.o \
${BINARY_FOLDER}/server.o \
${BINARY_FOLDER}/signed_strlen.o
	${CC} ${CFLAGS} $^ -o $@

${CLIENT_NAME_IN_FOLDER}: \
${BINARY_FOLDER}/client.o \
${BINARY_FOLDER}/common.o \
${BINARY_FOLDER}/parse_client_args.o \
${BINARY_FOLDER}/print.o \
${BINARY_FOLDER}/signed_strlen.o
	${CC} ${CFLAGS} $^ -o $@

# -- More boilerplate things --

.PHONY: ${NAME} all clean fclean re

clean:
	rm -rf ${BINARY_FOLDER}

fclean: clean
	rm -f ${SERVER_NAME_IN_FOLDER} ${CLIENT_NAME_IN_FOLDER}

re: fclean all
