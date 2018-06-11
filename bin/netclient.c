#undef NDEBUG
#include <stdlib.h>
#include <sys/select.h>
#include <stdio.h>
#include <ringbuffer.h>
#include <dbg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

struct tagbstring NL = bsStatic("\n");
struct tagbstring CRLF = bsStatic("\r\n");

// Sets up the socket file descriptor for Nonblocking I/O
int nonblock(int fd){
    int flags = fcntl(fd, F_GETFL, 0);	// Get the flags
    check(flags >= 0, "Invalid flags on nonblock.");

    int rc = fcntl(fd, F_SETFL, flags | O_NONBLOCK);	// Use those flags and also set it
    check(rc == 0, "Can't set nonblocking.");		// for Nonblocking I/O

    return 0;
error:
    return -1;
}

// Establish a new connection to the client specified by host:port
int client_connect(char *host, char *port){
    int rc = 0;
    struct addrinfo *addr = NULL;

    rc = getaddrinfo(host, port, NULL, &addr);		// Get an addrinfo structure to connect() to		
    check(rc == 0, "Failed to lookup %s:%s", host, port);

    int sock = socket(AF_INET, SOCK_STREAM, 0);		// Create a new socket for connecting to the  
    check(sock >= 0, "Cannot create a socket.");	// Internet using TCP

    rc = connect(sock, addr->ai_addr, addr->ai_addrlen);	// Connect to that socket with the specified
    check(rc == 0, "Connect failed.");				// addrinfo structure

    rc = nonblock(sock);				// Set Nonblocking on socket file descriptor
    check(rc == 0, "Can't set nonblocking.");

    freeaddrinfo(addr);
    return sock;

error:
    if(addr)
	    freeaddrinfo(addr);
    
    return -1;
}

// Read recieved data into the RingBuffer "buffer"
int read_some(RingBuffer * buffer, int fd, int is_socket){
    int rc = 0;

    if (RingBuffer_available_data(buffer) == 0) {
	    // BUG : use an interface here
        buffer->start = buffer->end = 0;
    }

    if (is_socket) {	// If it's a socket use recv() to read into the RingBuffer 
        rc = recv(fd, RingBuffer_starts_at(buffer),
                RingBuffer_available_space(buffer), 0);
    } else {		// else use read(). But here it's basically the same.
        rc = read(fd, RingBuffer_starts_at(buffer),
                RingBuffer_available_space(buffer));
    }

    check(rc >= 0, "Failed to read from fd: %d", fd);

    RingBuffer_commit_write(buffer, rc);

    return rc;

error:
    return -1;
}

// Write data from the RingBuffer to a file descriptor fd
int write_some(RingBuffer * buffer, int fd, int is_socket){
    int rc = 0;
    bstring data = RingBuffer_get_all(buffer);

    check(data != NULL, "Failed to get from the buffer.");
    check(bfindreplace(data, &NL, &CRLF, 0) == BSTR_OK,		// Not much essential
            "Failed to replace NL.");

    if (is_socket) {	// Same case as read_some
        rc = send(fd, bdata(data), blength(data), 0);
    } else {
        rc = write(fd, bdata(data), blength(data));
    }

    check(rc == blength(data), "Failed to write everything to fd: %d.",
            fd);
    bdestroy(data);

    return rc;

error:
    return -1;
}

int main(int argc, char *argv[]){
    fd_set allreads;
    fd_set readmask;

    int socket = 0;
    int rc = 0;
    RingBuffer *in_rb = RingBuffer_create(1024 * 10);		// Used for input from stdin
    RingBuffer *sock_rb = RingBuffer_create(1024 * 10);		// Used for input from socket

    check(argc == 3, "USAGE: netclient host port");

    socket = client_connect(argv[1], argv[2]);
    check(socket >= 0, "connect to %s:%s failed.", argv[1], argv[2]);

    FD_ZERO(&allreads);
    FD_SET(socket, &allreads);
    FD_SET(0, &allreads);

    while (1) {
        readmask = allreads;
        rc = select(socket + 1, &readmask, NULL, NULL, NULL);
        check(rc >= 0, "select failed.");

        if (FD_ISSET(0, &readmask)) {
            rc = read_some(in_rb, 0, 0);
            check_debug(rc != -1, "Failed to read from stdin.");
        }

        if (FD_ISSET(socket, &readmask)) {
            rc = read_some(sock_rb, socket, 1);
            check_debug(rc != -1, "Failed to read from socket.");
        }

        while (!RingBuffer_empty(sock_rb)) {
            rc = write_some(sock_rb, 1, 0);
            check_debug(rc != -1, "Failed to write to stdout.");
        }

        while (!RingBuffer_empty(in_rb)) {
            rc = write_some(in_rb, socket, 1);
            check_debug(rc != -1, "Failed to write to socket.");
        }
    }

    return 0;

error:
    if(in_rb)	RingBuffer_destroy(in_rb);
    if(sock_rb) RingBuffer_destroy(sock_rb);
    
    return -1;
}
