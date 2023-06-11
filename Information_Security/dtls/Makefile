all: client server

client:
	gcc helper.c client.c -lcrypto -lssl -o client

server:
	gcc helper.c server.c -lcrypto -lssl -o server

.PHONY: clean

clean:
	rm -f client server
