CC=gcc
TARGETS=hw0301 hw0302 hw0303 hw0304 hw0305
CFLAG=-lm

all:

	for name in ${TARGETS}; \
	do \
	${CC} $$name.c lrc.c ${CFLAG} -o $$name ;\
	done


clear:
	rm -rf ${TARGETS}
