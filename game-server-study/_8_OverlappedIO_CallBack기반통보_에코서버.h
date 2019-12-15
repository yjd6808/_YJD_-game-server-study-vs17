/*
 *@Author		: JungDo Yun
 *@Created		: 2019-12-15 오후 4:42:41
 *@Description	: 에코서버로 다시 만듬
 */

#pragma once

#include "common_header.h"
#include "Socket.h"

using namespace std;

struct Client {
	EndPoint endPoint;
	SOCKET socket;

	Client(SOCKET socket) {
		this->socket = socket;
		socklen_t retLength = sizeof(endPoint.addr_in);
		if (::getpeername(socket, (sockaddr*)&endPoint.addr_in, &retLength) < 0)
		{
			DWORD errorId = GetLastError();
			cout << "getPeerAddr failed:" << GetLastErrorAsString(errorId) << endl;
			throw exception();
		}
		if (retLength > sizeof(endPoint.addr_in))
		{
			cout << "getPeerAddr buffer overrun: " << retLength;
			throw exception();
		}
	}
};


unordered_map<SOCKET, Client*> clients;

class _8_OverlappedIO_CallBack기반통보_에코서버 {
private:
	static void CALLBACK SendCompletionRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {
		if (dwError != 0) {
			cout << "send completion routine error : " << GetLastErrorAsString(dwError);
			exit(-1);
		}

		OverlappedIOContext* sendContext = (OverlappedIOContext*)lpOverlapped;
		Client* client = (Client*)sendContext->data;

		cout << client->socket << "에게 " << cbTransferred << " 바이트의 메시지 전송완료" << endl;
		delete sendContext;
	}

	static void CALLBACK RecvCompletionRoutine(DWORD dwErrors, DWORD cbTransffered, LPOVERLAPPED lpOverlapped, DWORD dwflag) {
		OverlappedIOContext* recvContext = (OverlappedIOContext*)lpOverlapped;
		Client* client = (Client*)recvContext->data;

		if (cbTransffered <= 0) {
			cout << client ->socket << " 클라이언트 와의 연결이 끊어졌습니다" << endl;
			closesocket(client->socket);
			clients.erase(client->socket);
			delete recvContext;
			delete client;
			return;
		}

		cout << cbTransffered << "바이트 메세지 수신완료" << endl;
		cout << "내용 : " << recvContext->buffer << endl;

		OverlappedIOContext* sendContext = new OverlappedIOContext(IO_SEND);
		sendContext->SetSendBuffer(recvContext->buffer, strlen(recvContext->buffer) + 1);
		sendContext->data = client;

		DWORD sendByteCount = 0;

		WSASend(client->socket, &sendContext->wsaBuf, 1, &sendByteCount, 0, (OVERLAPPED*)sendContext, SendCompletionRoutine);
		DWORD errorID = WSAGetLastError();
		if (errorID != 0 && errorID != WSA_IO_PENDING) {
			cout << "Comp 송신 오류 : " << GetLastErrorAsString(errorID) << endl;
			return;
		}

		DWORD recvByteCount = 0;
		DWORD flag = 0;

		recvContext->SetRecvBuffer();
		WSARecv(client->socket, &recvContext->wsaBuf, 1, &recvByteCount, &flag, (OVERLAPPED*)recvContext, RecvCompletionRoutine);
		DWORD errorID2 = WSAGetLastError();
		if (errorID2 != 0 && errorID2 != WSA_IO_PENDING) {
			cout << "Comp 수신 오류 : " << GetLastErrorAsString(errorID2) << endl;
			return; 
		}
	}


	void instance_main() {
		ServerSocket socket(TransportLayerProtocol::TCP);
		socket.SetNonbloking();
		socket.Bind("127.0.0.1", 8500);
		socket.Listen();

		cout << "서버 구동완료" << endl;

		while (1) {
			SOCKET clientSocket = ::accept(socket.Get(), nullptr, nullptr);
			if (clientSocket == INVALID_SOCKET) {
				DWORD errorID = WSAGetLastError();
				if (errorID == WSAEWOULDBLOCK) {
					SleepEx(1, true);
					continue;
				}
				else {
					cout << "accept error occured : " << GetLastErrorAsString(errorID) << endl;
					return;
				}
			}

			cout << clientSocket << " 클라이언트가 들어왔습니다." << endl;
			Client* client = new Client(clientSocket);
			OverlappedIOContext* recvContext = new OverlappedIOContext(IO_RECV);
			recvContext->SetRecvBuffer();
			recvContext->data = client;

			clients.insert(pair<SOCKET, Client*>(clientSocket, client));
			DWORD recvByteCount;
			DWORD flag = 0;
			
			
			WSARecv(client->socket, &recvContext->wsaBuf, 1, &recvByteCount, &flag, (OVERLAPPED*)recvContext, RecvCompletionRoutine);
			DWORD errorID = WSAGetLastError();
			if (errorID != 0 && errorID != WSA_IO_PENDING) {
				cout << "수신 오류 : " << GetLastErrorAsString(errorID) << endl;
				exit(-1);
			}
		}
	}


public:
	static void main() {
		_8_OverlappedIO_CallBack기반통보_에코서버 instance;
		instance.instance_main();
	}
};

