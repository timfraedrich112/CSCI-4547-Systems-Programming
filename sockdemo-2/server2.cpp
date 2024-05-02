// server2.cpp - open a socket on port 1099 and use it to accept several clients.

#include "tools.hpp"
#include "Socket.hpp"

#define MAXCLIENTS 2				// Unrealistically small, for testing.
typedef struct pollfd  toPoll;

int doWelcome(int welcomeSock, int* nClip, toPoll* worker, const char* greeting);
int doService(toPoll* p);
int getPort(int fd);
void printsockaddr_in(const char* who, sockaddr_in sock);

int main(int argc, char *argv[]) {
	const char* proc = argv[0];
	if (argc<2) fatal("usage: %s port\n", proc);
	int port = strtol(argv[1], NULL, 10);
	
	char hostname[256];					// to store name of local host.
	gethostname(hostname, 256);			// get name of local host.
	cout <<"server is " <<hostname <<endl;
	
	int nCli = 0;						// number of working sockets still alive
	int welcomeFd;						// fd for the welcoming socket
	
	toPoll ufds[MAXCLIENTS + 1];		// These are the connections to poll.
	toPoll* const welcome = &ufds[0];	// entry for the welcome socket
	toPoll* const worker = &ufds[1];	// first working socket.
	
	int status;							// return values from system calls.
	
	char greeting[256];
	sprintf(greeting, "Connected to (%s.%d)\n", hostname, port);
	
	//===========================================================================
	// Create an internet socket in the file table; return the file descriptor.
	// Use INET protocol family, stream i/o, exact protocol irrelevant (== 0)
	Socket server;
	server.listen(port);
	welcomeFd = server.sockfd();

	// Install welcome socket in ufds[] polling table
	welcome->fd = welcomeFd;
	welcome->events = POLLIN;		// let socket listen for calls from clients

	printf("Opened socket as fd (%d) on port (%d) for stream i/o\n",
		   welcomeFd, ntohs(server.port()));
	cout << "welcome" << server;

	//===========================================================================
	// -------------------------------------------------------- main polling loop
	// Listen simultaneously to welcomeSock for new connections and to all of the
	// working sockets for data. Do not exit; servers operate in an infinite loop.
	//
	for (;;) {
		status = poll(ufds, 1 + nCli, -1);
		//printf("."); fflush(stdout);
		if (status < 0)  fatalp("Error in poll().\t");
		if (status == 0) sayp("%s: poll timed out\t", proc);
		// =====================================================================
		// If welcome socket has a caller, service it.
		if (welcome->revents != 0 )	{
			if (welcome->revents & POLLIN)	{
				if (nCli < MAXCLIENTS) // create a new client connection.
					nCli += doWelcome(welcomeFd, &nCli, worker, greeting);
            }
            else fatalp("Error involving welcome mat: %d", welcome->revents);
		}
		
		// =====================================================================
		// Scan the working sockets and process whatever tasks you find
		int k;
		for (k = 0; k < nCli; k++) {
			if (worker[k].revents != 0) {
				status = doService( &worker[k] );
				if ( status == -1){		// Remove dead socket from polling table
					worker[k] = worker[--nCli];		// decrement # of workers
				}
			}
		}
		// stop looking for new clients if max has been reached ----------------
		welcome->events = (nCli < MAXCLIENTS) ? POLLIN : 0;
	}	// end polling loop
}

//==============================================================================
// Returns 1 if connection succeeded; 0 if no new worker was created
int doWelcome(int welcomeSock, int* nClip, toPoll* worker, const char* greeting) {
	int nCli = *nClip;	// Make a local copy of the in/out parameter
	sockaddr_in newCaller;
	socklen_t sockLen = sizeof newCaller;
	
	int newfd = accept(welcomeSock, (sockaddr*) &newCaller, &sockLen);
	if (newfd < 0) {
		cout <<"False alarm: packet was rejected.\n";
		return 0;  // False alarm, nobody there, 0 new clients.
	}
	
	printsockaddr_in("working--from newCaller", newCaller);  // same as getpeername
	// --------------------------- Put new socket into our polling list.
	worker[nCli].fd = newfd;
	worker[nCli].events = POLLIN;
	worker[nCli].revents = 0;
	
	// ----------------------------- We have a new caller -- send an ack.
	int bytes = write(newfd, greeting, strlen(greeting));
	if (bytes < 1) say("Error while writing to socket");
	
	*nClip = nCli;		// Return the possibly-modified index of last client.
	return 1;
}

//==============================================================================
// Returns -1 if the connection was closed; 0 otherwise
int doService(toPoll* p) {
	char buf[BUFSIZ + 1];
	int retval = 0;		// Change in number of workers.
	
	if (p->revents == 0)  return -1;

		// ------------------------------------------- Test for a message event.
		if (p->revents & POLLIN) {// This is a read event--read it
			int bytes = read(p->fd, buf, (sizeof buf) - 1);
			// --------------------------------- We got a message, so handle it.
			if (bytes > 0) {		
				buf[bytes] = '\0';	// read does not null terminate.
				printf("\nFrom port %d:\t\t( %s ", getPort(p->fd), buf);
			} 
			// -----------------------No message, so handle the possible errors.
			else if (bytes == 0) {	// Indicates end of file.
				printf("closing socket on port %d\n", getPort(p->fd));
				close(p->fd);
				retval = -1;
			} else if (errno == ECONNRESET) {
				sayp("socket %d disappeared", getPort(p->fd));
				close(p->fd);
				retval = -1;
			} else {
				fatalp("Error %d from read, port %d", bytes, getPort(p->fd));
			}
		} 
		// ---------------------------- It wasn't a message, so test for hangup.
		else if (p->revents & POLLHUP) {  // Caller hung up.
			say("Removing dead socket %d\n", getPort(p->fd));
			close(p->fd);
			retval = -1;
		}	// end if p has events.
	
	return retval;
}

//==============================================================================
int getPort(int fd){
	sockaddr_in client;
	socklen_t sockLen = sizeof client;
	int status = getpeername(fd, (sockaddr*) &client, &sockLen);
	if (status<0) fatalp("Can't get port# of socket (%d)", fd);
	return ntohs(client.sin_port);
}

// =============================================================================
void printsockaddr_in(const char* who, sockaddr_in sock) {
	printf("%s socket is  {\n\t"
		   "sin_family        = %d\n\t"
		   "sin_addr.s_addr   = %s\n\t"
		   "sin_port          = %d\n\t"
		   "}\n", who, sock.sin_family, inet_ntoa(sock.sin_addr),
		   ntohs(sock.sin_port));
}

//=================================================================
/* History:
 Derived from isockl.c (C) 1991 Blair P. Houghton.
 Modified in 1993 to remove pre-ANSI code by Michael J. Fischer
 Modified 2010 to improve style and clarity by Michael J. and Alice E. Fischer
 Comments added 2012 by Alice E. Fischer.
 Copyright June 2012 by Michael J. Fischer and Alice E. Fischer
 //
 // Output:
 [alice] lily:~/<3>Lecture/L10-sockets/sockdemo-2~> server2 1099 localhost
 server is lily.local
 {
 sin_family       =
 sin_addr.s_addr  = 0.0.0.0
 sin_port         = 1099
 }
 Just bound socket 3:     {
 sin_family       =
 sin_addr.s_addr  = 0.0.0.0
 sin_port         = 1099
 }

 Just called listen(); now waiting for a client to show up
 Opened socket as fd (3) on port (1099) for stream i/o
 welcome    {
 sin_family       =
 sin_addr.s_addr  = 0.0.0.0
 sin_port         = 1099
 }
 working--from newCaller socket is  {
 sin_family        = 2
 sin_addr.s_addr   = 127.0.0.1
 sin_port          = 51909
 }
 working--from newCaller socket is  {
 sin_family        = 2
 sin_addr.s_addr   = 127.0.0.1
 sin_port          = 51910
 }

 From port 51909:        ( Politeness, by Alan Alexander Milne

 From port 51910:        ( There was a little girl

 From port 51909:        ( When people ask me, I always tell them:

 From port 51910:        ( who had a little curl

 From port 51909:        ( "Quite well, thank you, I'm very glad to say."

 From port 51910:        ( right in the middle of her forehead.

 From port 51909:        ( If people ask me, I always answer,

 From port 51910:        ( When she was good she was very very good,

 From port 51909:        ( "Quite well, thank you, how are you to-day?"

 From port 51910:        ( and when she was bad, she was horrid.
 closing socket on port 51910
 working--from newCaller socket is  {
 sin_family        = 2
 sin_addr.s_addr   = 127.0.0.1
 sin_port          = 51911
 }

 From port 51909:        ( I always answer,
 From port 51911:        ( Happiness, by Alan Alexander Milne

 From port 51909:        ( I always tell them,

 From port 51911:        ( John had

 From port 51909:        ( If they ask me
 From port 51911:        ( Great Big

 From port 51909:        ( Politely.....

 From port 51911:        ( Waterproof

 From port 51909:        ( BUT SOMETIMES

 From port 51911:        ( Boots on;

 From port 51909:        ( I wish

 From port 51911:        ( John had a

 From port 51909:        ( That they wouldn't.
 closing socket on port 51909

 From port 51911:        ( Great Big

 From port 51911:        ( Waterproof

 From port 51911:        ( Hat;

 From port 51911:        ( John had a

 From port 51911:        ( Great Big

 From port 51911:        ( Waterproof

 From port 51911:        ( Mackintosh --

 From port 51911:        ( And that

 From port 51911:        ( (Said John)

 From port 51911:        ( Is

 From port 51911:        ( That.
 closing socket on port 51911


*/
