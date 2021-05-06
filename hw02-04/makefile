CC=gcc
TARGETS=hw0401 hw0402 hw0403 hw0404 hw0405
CFLAG=-lm -Wformat-zero-length

all:

	for name in ${TARGETS}; \
	do \
	${CC} $$name.c ${CFLAG} myprintf.c -o $$name ;\
	done


clear:
	rm -rf ${TARGETS}
