CC=gcc
TARGETS=hw0301 hw0302 hw0303 hw0304
CFLAG=-lm

all:

	for name in ${TARGETS}; \
	do \
	${CC} $$name.c lrc.c ${CFLAG} -o $$name ;\
	done

	${CC} hw0305.c ${CFLAG} gui.c -o hw0305 `pkg-config --cflags --libs gtk+-3.0`

clear:
	rm -rf ${TARGETS}
	rm -rf hw0305