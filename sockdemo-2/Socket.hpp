/*
 * Socket.hpp
 * Many socket details are collected by this class, enabling the server and client modules to operate at a higher level.
 * Created on: Oct 20, 2017
 *     Author: Michael Fischer
 */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <unistd.h>
#include <netdb.h>
#include <sys/poll.h>
#include <arpa/inet.h>

#include "tools.hpp"

// Object of specified family, type, and protocol
class Socket {
private:
	int fd = -1;	// File descriptor for this socket
	sockaddr_in info = {0};

public:
	Socket();
	in_port_t   port()   { return info.sin_port; }
	in_addr     addr()   { return info.sin_addr; }

	int sockfd() const { return fd; }
	void refresh();
	int peer(Socket* ps) const;

	void listen(int port);
	void connect(const char* host, int port);

	ostream& print(ostream& out) const;
	ostream& printPeer(ostream& out) const;
};

inline ostream& operator <<(ostream& out, const Socket& sn) {
	return sn.print(out); }

#endif /* SOCKET_HPP_ */
