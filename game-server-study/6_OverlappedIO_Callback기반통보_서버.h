#pragma once

#include <WinSock2.h>
#include <thread>
#include <iostream>
#include <Ws2tcpip.h>
#include <vector>
#include "Util.h"

using namespace std;

#define BUF_SIZE 1024



class _6_OverlappedIO_Callback����뺸_���� {
	struct Client {
		SOCKET clientSocket;
		WSABUF recvBuf;
		WSAOVERLAPPED clientOverlapped;
		WSAEVENT dummyEvent;
		SOCKADDR_IN clientAddress;
		char recvBuffer[BUF_SIZE];
		DWORD lastErrorID;
		DWORD recvBytesCount;
		DWORD flag;

		Client(SOCKET socket) {
			clientSocket = socket;
			recvBuf.buf = recvBuffer;
			recvBuf.len = BUF_SIZE;
			ZeroMemory(&clientOverlapped, sizeof(clientOverlapped));
			recvBytesCount = 0;
			flag = 0;
			lastErrorID = 0;
			dummyEvent = WSACreateEvent();
			clientOverlapped.hEvent = dummyEvent;
		}
	};

	SOCKET serverSocket;
	SOCKADDR_IN serverAddress;
	vector<Client*> clientVec;
	vector<thread> threadVec;
private:
	void WSAInit() {
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	}

	void ServerSocketInit() {
		serverSocket = WSASocket(PF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED );
	}

	void ServerAddressInit() {
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(9000);
		inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
	}

	void ServerBind() {
		if (::bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
			cout << "���� ���ε� ����" << endl;
			exit(-1);
		}
	}

	void ServerListen() {
		if (::listen(serverSocket, 5000) == SOCKET_ERROR) {
			cout << "���� ���� ����" << endl;
			exit(-1);
		}
	}

	static void CALLBACK CompRoutine(DWORD dwError, DWORD recvBytesCount, LPWSAOVERLAPPED lpOverlapped, DWORD flags) {
		
	}

	void WaitForClient() {
		while (true) {
			SOCKADDR_IN clientAddress;
			int clientAddressSize = sizeof(clientAddress);
			SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &clientAddressSize);

			Client* client = new Client(clientSocket);
			memcpy(&client->clientAddress, &clientAddress, sizeof(SOCKADDR_IN));
			clientVec.push_back(client);

			WSARecv(clientSocket, &client->recvBuf, 1, &client->recvBytesCount, &client->flag, &client->clientOverlapped, CompRoutine);
			client->lastErrorID = WSAGetLastError();
			if (client->lastErrorID != ERROR_IO_PENDING) {
				cout << "���� RecvError ���� : " << GetLastErrorAsString(client->lastErrorID) << endl;
				exit(-1);
			}

			threadVec.emplace_back([&client]
			{

			});
		}
	}


	void worker() {
		WSAInit();
		ServerSocketInit();
		ServerAddressInit();
		ServerBind();
		ServerListen();
		WaitForClient();
	}

public:
	static void main() {
		_6_OverlappedIO_Callback����뺸_���� instance;
		thread th(instance.worker);
	}
};