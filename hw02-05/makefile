CC=gcc
TARGETS=hw0501 hw0502 hw0503
CFLAG=-lm -Wformat-zero-length

all:

	for name in ${TARGETS}; \
	do \
	${CC} $$name.c ${CFLAG} -o $$name ;\
	done


clear:
	rm -rf ${TARGETS}
