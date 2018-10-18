CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -fPIC -fvisibility=hidden
LDFLAGS = -shared
TARGET_LIB = libmalloc.so
OBJS = src/malloc.o

.PHONY: all ${TARGET_LIB} clean check

${TARGET_LIB}: ${OBJS}
	${CC} ${LDFLAGS} -g3 -o $@ $^

all: ${TARGET_LIB}

check:
	make -B
	sh ./tests/run_tests.sh

clean:
	${RM} ${TARGET_LIB} ${OBJS}
