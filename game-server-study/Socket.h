/*
 *@Author  : JungDo Yun
 *@Created : 19-12-14 : ���� 9�� 59��
 */

#pragma once

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>

#define BUF_SIZE 4096

enum IOType
{
	IO_NONE,
	IO_SEND,
	IO_RECV,
	IO_RECV_ZERO,
	IO_ACCEPT
};

//���۰��� ��������
enum class TransportLayerProtocol {
	TCP,
	UDP
};

//�ּ� �����ϴ� Ŭ����
struct EndPoint {
	SOCKADDR_IN addr_in;
	std::string addr;
	short port;

	EndPoint() {}
	EndPoint(const char* addr, const short port);
	~EndPoint();

	std::string ToString();

	EndPoint& operator=(EndPoint& endPoint);
	EndPoint& operator=(EndPoint&& endPoint);
};

struct OverlappedIOContext
{
	OverlappedIOContext(IOType ioType) : ioType(ioType)
	{
		ZeroMemory(&overlapped, sizeof(OVERLAPPED));
		ZeroMemory(&wsaBuf, sizeof(WSABUF));
		ZeroMemory(buffer, BUF_SIZE);
		data = nullptr;
	}

	void SetSendBuffer(char* data, int dataLen) {
		ZeroMemory(&wsaBuf, sizeof(wsaBuf));
		memcpy(buffer, data, dataLen);
		wsaBuf.buf = buffer;
		wsaBuf.len = dataLen;
	}

	void SetRecvBuffer() {
		ZeroMemory(&wsaBuf, sizeof(wsaBuf));
		ZeroMemory(buffer, BUF_SIZE);
		wsaBuf.buf = buffer;
		wsaBuf.len = BUF_SIZE;
	}

	OVERLAPPED		overlapped;
	void*			data;
	IOType			ioType;
	WSABUF			wsaBuf;
	char			buffer[BUF_SIZE];
};

class Socket {
public:
	SOCKET Get() { return socket; }
protected:
	Socket(TransportLayerProtocol transportLayerProtocol);
protected:
	SOCKET socket;
	TransportLayerProtocol transportLayerProtocol;
};

class ServerSocket : public Socket{
public:
	ServerSocket(TransportLayerProtocol transportLayerProtocol);
	~ServerSocket();

	void Bind(const char* addr, const short port);
	void Listen();
	void SetNonbloking();
private:
	EndPoint endPoint;
};

class ClientSocket : public Socket {
public:
	ClientSocket(TransportLayerProtocol transportLayerProtocol);
	~ClientSocket();

	void Connect(const char* addr, const short port);
private:
	EndPoint* connectedEndPoint;
};

