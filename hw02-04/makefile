CC=gcc
TARGETS=hw0401 hw0402  
CFLAG=-lm -Wformat-zero-length

all:

	for name in ${TARGETS}; \
	do \
	${CC} $$name.c ${CFLAG} myprintf.c -o $$name ;\
	done
	gcc hw0403.c myprintf.c -o hw0403
	gcc hw0404.c zipreader.c -o hw0404


clear:
	rm -rf ${TARGETS}
	rm -rf hw0403
	rm -rf hw0404
	rm -rf hw0405
