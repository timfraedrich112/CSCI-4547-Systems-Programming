/*
 *  sockTypes.hpp
 *  Created by Alice Fischer on 3/15/10.
 *  Useful things for working with sockets.
 */
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>
#include <netdb.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct pollfd		toPoll;

//typedef struct connection {
//	int		  worker;	// fd of working socket
//	sockInfo  caller;	// client's socket info
//	socklen_t fromlen;  // client's socket length
//	bool	  active;	// true if client is still around
//} connection;


/* ----------------------------------------- Type definitions for reference only
// Definition of sockInfo on my computer.  The Linux definition starts with
// the family and is written in terms of a lot of macros.
 struct sockaddr_in {
     __uint8_t       sin_len;
     sa_family_t     sin_family;
     in_port_t       sin_port;
     struct  in_addr sin_addr;
     char            sin_zero[8];
 };
 
 // Pointers to sockaddr_in and other socket address structures are cast to 
 // pointers of this type before use in various system calls:
 //
 struct sockaddr {
     unsigned short    sa_family;    // address family, AF_xxx
     char              sa_data[14];  // 14 bytes of protocol address
 };
 
 struct in_addr {
     in_addr_t s_addr;		// unsigned int (32 bit).
 };
 
 -- Definition of hostInfo
 struct  hostent {
    char    *h_name;        // official name of host 
    char    **h_aliases;    // alias list  
    int     h_addrtype;     // host address type  
    int     h_length;       // length of address  
    char    **h_addr_list;  // list of addresses from name server  
 };

 -- Definition of the object to be polled.
 struct pollfd {
	int    fd;       // file descriptor
	short  events;   // types of events to service
	short  revents;  // unread events
 };

 -- Prototypes of the new functions used in this example.
 
 int socket(int domain, int type, int protocol);
 int bind(int socket, const sockUnion* address, socklen_t address_len);
 int getsockname(int socket, sockUnion* address, socklen_t* address_len);
 int accept(int socket, sockUnion* restrict address, socklen_t* address_len);
 hostInfo*  gethostbyname(const char *name);
 int connect(int socket, const sockUnion* address, socklen_t address_len);

 void* memmove( void* toAddress, const void* fromAddress, size_t nBytes);
 ssize_t read(int fileDescriptor, void* buffer, size_t nBytes);
 ssize_t write(int fileDescriptor, const void* buf, size_t nBytes);
 FILE* fdopen(int fileDescriptor, const char* mode);.
*/
