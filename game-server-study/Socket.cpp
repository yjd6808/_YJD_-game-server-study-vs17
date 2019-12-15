#include "Socket.h"

#include <iostream>

using namespace std;

class WSAManager {
public:
	static void Init() {
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	}
	static void CleanUp() {
		WSACleanup();
	}
};

EndPoint::EndPoint(const char * address, const short port)
{
	int addressLen = strlen(address);
	this->addr = address;
	this->port = port;

	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(port);
	inet_pton(AF_INET, addr.c_str(), &addr_in.sin_addr);
}

EndPoint::~EndPoint()
{
}

std::string EndPoint::ToString()
{
	char addrString[200];
	addrString[0] = 0;
	inet_ntop(AF_INET, &addr_in.sin_addr, addrString, sizeof(addrString) - 1);

	char finalString[200];
	sprintf(finalString, "%s:%d", addrString, htons(addr_in.sin_port));
	return finalString;
}

EndPoint & EndPoint::operator=(EndPoint & endPoint)
{
	this->addr = endPoint.addr;
	this->port = endPoint.port;
	memcpy(&this->addr_in, &endPoint.addr_in, sizeof(SOCKADDR_IN));
	return *this;
}

EndPoint & EndPoint::operator=(EndPoint && endPoint)
{
	this->addr = endPoint.addr;
	this->port = endPoint.port;
	memcpy(&this->addr_in, &endPoint.addr_in, sizeof(SOCKADDR_IN));
	return *this;
}



Socket::Socket(TransportLayerProtocol transportLayerProtocol)
{
	WSAManager::Init();
	if (transportLayerProtocol == TransportLayerProtocol::TCP)
		socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	else
		socket = WSASocket(PF_INET, SOCK_DGRAM, IPPROTO_UDP, nullptr, 0, WSA_FLAG_OVERLAPPED);
}

ServerSocket::ServerSocket(TransportLayerProtocol transportLayerProtocol)  : Socket(transportLayerProtocol)
{
}

void ServerSocket::Bind(const char * addr, const short port)
{
	endPoint = EndPoint(addr, port);

	if (::bind(socket, (SOCKADDR*)&endPoint.addr_in, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "server bind error" << endl;
		exit(-1);
	}
}

void ServerSocket::Listen()
{
	if (::listen(socket, 5000) == SOCKET_ERROR) {
		cout << "server listen error" << endl;
		exit(-1);
	}
}

void ServerSocket::SetNonbloking()
{
	int mode = 1;
	::ioctlsocket(socket, FIONBIO, (u_long*)&mode);
}

ServerSocket::~ServerSocket()
{
}

ClientSocket::ClientSocket(TransportLayerProtocol transportLayerProtocol) : Socket(transportLayerProtocol)
{
}

ClientSocket::~ClientSocket()
{
	delete connectedEndPoint;
}

void ClientSocket::Connect(const char * addr, const short port)
{
	connectedEndPoint = new EndPoint(addr, port);

	if (::connect(socket, (SOCKADDR*)&connectedEndPoint->addr_in, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		cout << "connect faild" << endl;
		exit(-1);
	}
}
