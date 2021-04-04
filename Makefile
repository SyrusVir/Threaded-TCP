FIFOPATH = ~/Projects/General-FIFO
LIBFLAGS = -lfifo -pthread
CCFLAGS = -I $(FIFOPATH) -I. -L $(FIFOPATH) $(LIBFLAGS)

libtcphandler.a: tcp_handler.c 

tcp_handler.c: tcp_handler.h
	gcc -g -Wall -c $@ -I. $(CCFLAGS)

tcp_test: tcp_client_test tcp_server_test

tcp_client_test: tcp_handler.c
	gcc $@.c $^ -o $@.out $(CCFLAGS)

tcp_server_test: tcp_handler.c
	gcc $@.c $^ -o $@.out $(CCFLAGS)