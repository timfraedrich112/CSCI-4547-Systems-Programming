// clientMary.cpp -- open an internet socket and talk into it.
#include "tools.hpp"    // for fatal.
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

typedef struct sockaddr_in  sockInfo;  // Define sensible names.
typedef struct sockaddr     sockUnion;
typedef struct hostent      hostInfo;
#define PORT 1099

void printSockInfo( const char* who, sockInfo sock );

const char* const line[] = {
	"There was a little girl\n",
	"who had a little curl\n",
	"right in the middle of her forehead.\n",
	"When she was good she was very very good,\n",
	"and when she was bad, she was horrid.\n"
};
const int nLines = sizeof line / sizeof line[0];


// ---------------------------------------------------------------
// arg 1 is remote host.
int main( int argc, char *argv[] )
{
    // define meaningful names for the command-line arguments.
    char* proc = argv[0];
    char* servername = argv[1];  // registered network name of the server.
                                 // check correct number of arguments
    if ( argc != 2 ) fatal( "usage: %s hostname\n", proc );

    char buf[BUFSIZ+1];
    int nBytes, status;

    // ===========================================================
    // Install a socket in the client's file table.
    // Make an internet-transmitted, file-i/o-style, protocol-whatever plug
    int clientSock = socket( AF_INET, SOCK_STREAM, 0 );
    if ( clientSock<0 ) fatal( "%s: can't assign fd for client socket", proc );

    // ===========================================================
    // Use a domain name server to get the IP address associated with the host.
    // The name server returns a list of IP addresses.  We want the first one.
    hostInfo* remoteHost = gethostbyname( servername );
    if(remoteHost == nullptr) fatal("%s: unknown host: %s\n", proc, servername);

    // ===========================================================
    // This series of system calls sets up the client's socket structure.
    sockInfo clientInfo;
    clientInfo.sin_family = AF_INET;    // Address family internet

    // Copy info about the server into the client's socket structure.
    memmove( &clientInfo.sin_addr, remoteHost->h_addr_list[0], remoteHost->h_length);

    // Copy the agreed-upon port number into the client's socket structure.
    // Convert it first from the local representation to the internet standard.
    clientInfo.sin_port = htons( PORT );
    printSockInfo( "client", clientInfo );

    // ===========================================================
    // Info in the client struct is now complete and ready to connect to server.
    status = connect( clientSock, (sockUnion*) &clientInfo, sizeof clientInfo );
    if ( status < 0 ) fatal("%s: Connection to %s refused.", proc, servername);
    cout <<proc <<": connection established to " <<servername <<".\n";

    // wait for server to acknowledge the connection.
    nBytes = read( clientSock, buf, sizeof buf );
    if( nBytes >= 0 )  cout <<nBytes <<"  " <<buf;    // the connection message.
    else fatal("%s: Error while reading from socket.", proc );

    // Write lines until message is complete.
    for( int k = 0; k < nLines; k++ ) {
        sleep(1);
        nBytes = write( clientSock, line[k], strlen(line[k]) );
        cout <<"@";
        if( nBytes < 0 ) fatal("%s: Error while writing to socket.", proc);
    }
    cout <<endl;
    // all the socket connections are closed automatically
    exit(0);
}

// ================================================================
void printSockInfo( const char* who, sockInfo sock ){
    cout <<who <<" socket is  {\n\t"
    <<" sin_family      = " <<sock.sin_family <<"\n\t"
    <<" sin_addr.s_addr = " <<inet_ntoa (sock.sin_addr) <<"\n\t" //Linux: ntop
    <<" sin_port (!!!)  = " <<ntohs(sock.sin_port) <<"\n\t};\n";
}

//=================================================================
/* History:
 Derived from isockl.c (C) 1991 Blair P. Houghton.
 Modified in 1993 to remove pre-ANSI code by Michael J. Fischer
 Modified 2010 to improve style and clarity by Michael J. and Alice E. Fischer
 Comments added 2012 by Alice E. Fischer.
 Copyright March 2010 by Michael J. Fischer and Alice E. Fischer
 //
 //Output:
 lily:~/<3>Lecture/L10-sockets/sockdemo-2~> clientMary localhost
 client socket is  {
 sin_family      =
 sin_addr.s_addr = 127.0.0.1
 sin_port (!!!)  = 1099
 };
 clientMary: connection established to localhost.
 31  Connected to (lily.local.1099)
 @@@@@
 [alice] lily:~/<3>Lecture/L10-sockets/sockdemo-2~>*/
