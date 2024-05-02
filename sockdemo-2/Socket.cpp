/*
 * Socket.cpp
 *  Created on: Oct 20, 2017
 *      Author: mike
 */

#include "Socket.hpp"
// --------------------------------------------------
// Create Socket
Socket::Socket() {
    // Allocate a socket (kernel object) in file table.
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd<0 ) fatalp("Socket: Can't create socket");
}

// --------------------------------------------------
// Update structure with current socket info
void Socket::refresh() {
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int status = getsockname( fd, (sockaddr*)&info, &addrlen);
    if ( status < 0 ) fatalp("Socket: getsockname failed on socket %d.", fd);
    cout << *this;
}

// --------------------------------------------------
// Returns peer of connected socket
int
Socket::peer(Socket* ps) const {
    socklen_t addrlen = sizeof(sockaddr_in);
    sockaddr_in peeraddr; // Send to getpeername as an output parameter.
    int status = getpeername(fd, (sockaddr*)&peeraddr, &addrlen);
    if (addrlen != sizeof(sockaddr_in))
        fatal("Buffer length error %d!=%d\n", addrlen, sizeof(sockaddr_in));

    *(sockaddr_in*)ps = peeraddr;
    return status;
}

// --------------------------------------------------
void
Socket::listen(int port) {
	info.sin_family = AF_INET;
	info.sin_port = htons(port);
	info.sin_addr.s_addr = INADDR_ANY;

    // sockaddr is used to set fields of kernel socket.
    int status = ::bind(fd, (sockaddr*)&info, sizeof(struct sockaddr_in));
    if (status < 0) fatalp("Can't bind socket (%d)", fd);
    refresh();
    cout << "Just bound socket " << fd << ": " << *this << endl;

    // Declare that this is the welcome socket and it listens for clients.
    status = ::listen(fd, 1);
    if (status < 0) fatalp("Socket: Unable to listen on socket %d.", fd);
    cout << "Just called listen(); now waiting for a client to show up\n";
}

// --------------------------------------------------
void
Socket::connect(const char* host, int port) {
	info.sin_family = AF_INET;
	info.sin_port = htons(port);

	// Use domain name server to get IP address associated with server.
	struct hostent* remoteHost = gethostbyname(host);
	if (remoteHost == NULL) fatalp("Socket: unknown host: %s\n", host);

// Copy info about the host into this Socket's base class.
// The name server returns a list of IP addresses.  We
// want the first. h_addr_list[0] is the first address
// found by gethostbyname().  h_length is the length of
// addresses in the h_addr_list[].
   memmove(&info.sin_addr, remoteHost->h_addr_list[0], remoteHost->h_length);

// Copy the agreed-upon port number into this Socket's
// base class. Convert first from the local
// representation to the internet standard.
	info.sin_port = htons(port);

// Info in client sockaddr struct is now ready to connect to server.
	cout << "Ready to connect socket " << fd << " to " << host << endl;
	int status = ::connect(fd, (sockaddr*)&info, sizeof(struct sockaddr_in));
	if (status < 0) fatalp("Client: Connection to %s refused.", host);
	refresh();
	cout << "Socket: connection established to " << host << ".\n";
}

// --------------------------------------------------
ostream&
Socket::print(ostream& out) const {
    out <<  "\t{\n"
    "\tsin_family       = " << info.sin_family << "\n"
    "\tsin_addr.s_addr  = " << inet_ntoa(info.sin_addr) << "\n"
    "\tsin_port         = " << ntohs(info.sin_port) << "\n"
    "\t}\n";
    return out;
}

// --------------------------------------------------
ostream&
Socket::printPeer(ostream& out) const {
    Socket ps;
    int status = peer(&ps);
    if (status==0)  out <<  "Peer socket:\n" << ps << endl;
    else            out << "\tCan't get peer." << strerror(errno) << "\n";
    out << "\t}\n";
    return out;
}
