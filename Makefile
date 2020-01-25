all: udpclient udpserver

udpclient: udpclient.c
	gcc -Wall -g -o udpclient udpclient.c

udpserver: udpserver.c
	gcc -Wall -g -o udpserver udpserver.c

clean: 
	rm -f udpclient
	rm -f udpserver

