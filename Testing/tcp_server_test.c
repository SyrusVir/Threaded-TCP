#include "tcp_handler.h"
#include <arpa/inet.h>

#define PORT 49217

int main() 
{   
    //Configure server socket that listens for incoming client connections
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; //TCP
	server_address.sin_port = htons(PORT); //define port
	server_address.sin_addr.s_addr = INADDR_ANY; //accept connection at any address available

    //Initialize TCP Handler
    tcp_handler_t* tcp_handler = tcpHandlerInit(server_address, 150);

    //start Consumer loop
    pthread_t tcp_handler_tid;
    pthread_create(&tcp_handler_tid, NULL, &tcpHandlerMain, tcp_handler);

    printf("tcpHandler state post pthread_create=%d\n", tcp_handler->tcp_state);

    printf("s = send message\n");
    printf("d = disconnect from client\n");
    printf("c = close server\n");
    
    char c;
    char send_msg[20] = "hello world";
    do
    {
        scanf("%c", &c);
        switch (c)
        {
            case 's':
                printf("tcpHandlerWrite status = %d\n", tcpHandlerWrite(tcp_handler, send_msg, sizeof(send_msg),0,1));
                break;
            case 'c':
                printf("tcpHandlerClose status = %d\n", tcpHandlerClose(tcp_handler, 0, 1));
                break;
            case 'd':
                printf("tcpHandlerDisconnect status = %d\n", tcpHandlerDisconnect(tcp_handler, 0, 1));
                break;
        }
    } while (c != 'c');

    pthread_join(tcp_handler_tid, NULL);
    tcpHandlerDestroy(tcp_handler);
} //end main()