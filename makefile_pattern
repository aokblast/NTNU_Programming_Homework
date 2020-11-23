CC=gcc
TARGETS=hw0301 hw0302 hw0303 hw0304 hw0305
CFLAG=-lm -Wformat-zero-length

all:

	for name in ${TARGETS}; \
	do \
	${CC} $$name.c ${CFLAG} -o $$name ;\
	done


clear:
	rm -rf ${TARGETS}
