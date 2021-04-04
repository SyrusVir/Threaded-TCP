#include "fifo.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#include <netinet/in.h>
#include <netinet/tcp.h>

#define START_TYPE char                 // data type of the framing data denoting start of TCP data packet
#define START_BYTES sizeof(START_TYPE)  // number of bytes in framing data that denotes beginning of TCP data packet
#define START_DATA '|'                  // data that constitute the beginning of a TCP data packet

#define HEADER_TYPE uint32_t    // Header contains number of data bytes in TCP data packet; use data type to define header length 
#define HEADER_BYTES sizeof(HEADER_TYPE) // size of the header data type; used

typedef enum TcpHandlerCMD {
    TCPH_WRITE,
    TCPH_DISCONNECT,
    TCPH_CLOSE
} tcp_cmd_t;

typedef enum TcpHandlerState {
    UNCONNECTED,
    CONNECTED,
    ERROR
} tcp_state_t;

typedef struct TcpHandlerMsg {
    tcp_cmd_t CMD;
    void* data;
    size_t data_len;
} tcp_msg_t;

typedef struct TcpHandler {
    int client_socket;
    struct sockaddr_in server_address;  //address to place socket that listens for client connections
    tcp_state_t tcp_state;              //current state of the tcp_handler; check from producer to see if ready for writes
    fifo_buffer_t* write_buffer;        //fifo buffer holding data to write
} tcp_handler_t;

//Returns pointer to allocated memory space holding a TCP Handler message struct.
//A pointer to a copy of the contents of data is used in the message struct
tcp_msg_t* tcpMsgCreate(tcp_cmd_t CMD, void* data, size_t data_len);

//De allocates memory allocated for the 'data' member of the msg struct as well as the
//memory for the msg struct itself
void tcpMsgDestroy(tcp_msg_t* tcp_msg);

//Configures TCP keep alive 
int tcpConfigKeepalive(int socket, int idle_time_sec, int num_probes, int probe_intvl_sec);

//Initializes a TCP Handler to listen for client connections at server_address
tcp_handler_t* tcpHandlerInit(struct sockaddr_in server_address, int max_buffer_size);

//Destroys memory allocated by tcpHandlerInit; Returns any messages remaining in its command message queue
tcp_msg_t** tcpHandlerDestroy(tcp_handler_t* tcp_handler) ;

//Sends data to the client socket currently connected to the handler
int tcpHandlerWrite(tcp_handler_t* tcp_handler, void* data, size_t data_len, int priority, bool blocking);

//Sends close message to the tcp_handler; Current connections are closed and its executing thread stops.
int tcpHandlerClose(tcp_handler_t* tcp_handler, int priority, bool blocking);

//Sends tcp handler command to disconnect from the current client connection
int tcpHandlerDisconnect(tcp_handler_t* tcp_handler, int priority, bool blocking);

//Main Consumer loop to pass to pthread_create
void* tcpHandlerMain(void* tcpHandler_void);