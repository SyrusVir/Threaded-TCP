CC=gcc
CCFLAGS = -g -O -Wall -Wextra
FIFOPATH = $(CURDIR)/Generic-FIFO
LIBFLAGS = -pthread
ARFLAGS = rcs
FIFOOBJ = $(FIFOPATH)/fifo.o

libtcphandler.a: tcp_handler.o $(FIFOOBJ)
	ar $(ARFLAGS) libtcphandler.a $^

tcp_handler.o: tcp_handler.c tcp_handler.h
	$(CC) $(CCFLAGS) -c $< -I. $(LIBFLAGS)

*fifo.o:
	$(MAKE) -C $(FIFOPATH) $@

.PHONY clean:
clean:
	rm -f *.o *.a *.gch && $(MAKE) -C $(FIFOPATH) clean


tcp_test: tcp_client_test tcp_server_test

tcp_client_test: tcp_handler.c
	gcc $@.c $^ -o $@.out -I. -I$(FIFOPATH) $(LIBFLAGS)

tcp_server_test: tcp_handler.c
	gcc $@.c $^ -o $@.out $-I. -I(FIFOPATH) $(LIBFLAGS)