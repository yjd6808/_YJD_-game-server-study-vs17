/*
 *@Author		: JungDo Yun
 *@Created		: 2019-12-17 오후 1:18:07
 *@Description	: IOCP를 활용해서 만듬 / 내가 만든 어떠한 라이브러리도 쓰지 않고 다시 맨땅부터 시작
 */

#pragma once

#include "common_header.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <concurrent_unordered_map.h>

using namespace std;

#define MAX_CONCURRENT_IO_THREAD_COUNT	10
#define BUF_SIZE	1024


enum IOType {
	IO_RECV,
	IO_SEND,
	IO_STOP,
	IO_DEFAULT
};

void printError(std::string title, DWORD errorId) {
	cout << title << "(" << errorId << ")" << GetLastErrorAsString(errorId);
}

struct OverlappedIO : public OVERLAPPED {
	WSABUF buf;
	IOType ioType;
	void* data;

	OverlappedIO() {
		ZeroMemory(&buf, sizeof(WSABUF));
		ioType = IO_DEFAULT;
		data = nullptr;
	}
};

struct Client {
	SOCKET socket;
	SOCKADDR_IN addr_in;

	char recvBuffer[BUF_SIZE];
	char sendBuffer[BUF_SIZE];

	OverlappedIO recvOverlapped;
	OverlappedIO sendOverlapped;

	bool isConnected;
	std::mutex clientMtx;

	Client(SOCKET clientSocket) {
		isConnected = true;
		socket = clientSocket;
		ZeroMemory(recvBuffer, BUF_SIZE);
		ZeroMemory(sendBuffer, BUF_SIZE);
		ZeroMemory(&recvOverlapped, sizeof(OverlappedIO));
		ZeroMemory(&sendOverlapped, sizeof(OverlappedIO));
	}
};

class ClientManager {
	static ClientManager* instance;

	concurrency::concurrent_unordered_map<SOCKET, Client*> clientList;
	std::mutex clientListMtx;
	
public:
	static ClientManager* getInstance() {
		if (instance == nullptr) {
			instance = new ClientManager();
		}

		return instance;
	}

	void addClient(Client* client) {
		clientList.insert(pair<SOCKET, Client*>(client->socket, client));
	}

	void removeClient(Client* client) {
		lock_guard<mutex> clienListLocker(clientListMtx);
		clientList.unsafe_erase(client->socket);
	}

	void disconnect(Client* client) {
		lock_guard<mutex> clientLocker(client->clientMtx);
		client->isConnected = false;
	}

	void broadcast(Client* client, char* data, int len) {
		lock_guard<mutex> clienListLocker(clientListMtx);

		for (auto iter = clientList.begin(); iter != clientList.end(); iter++) {
			if (client->socket == iter->first)
				continue;

			postSend(iter->second, data, len);
		}
	}

	void postRecv(Client* client) {
		lock_guard<mutex> clientLocker(client->clientMtx);
		DWORD recvByteCount = 0;
		DWORD wsaRecvFlag = 0;
		client->recvOverlapped.buf.buf = client->recvBuffer;
		client->recvOverlapped.buf.len = BUF_SIZE;
		client->recvOverlapped.ioType = IO_RECV;
		client->recvOverlapped.data = client;
		if (WSARecv(client->socket, &client->recvOverlapped.buf, 1, &recvByteCount, &wsaRecvFlag, (OVERLAPPED*)&client->recvOverlapped, nullptr) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				client->isConnected = false;
			}
		}
	}

	void postSend(Client* client, char* data, const int len) {
		lock_guard<mutex> clientLocker(client->clientMtx);
		DWORD sendByteCount = 0;
		memcpy(client->sendBuffer, data, len);
		client->sendOverlapped.buf.buf = client->sendBuffer;
		client->sendOverlapped.buf.len = len;
		client->sendOverlapped.ioType = IO_SEND;
		client->sendOverlapped.data = client;
		if (WSASend(client->socket, &client->sendOverlapped.buf, 1, &sendByteCount, 0, (OVERLAPPED*)&client->sendOverlapped, nullptr) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING)
				client->isConnected = false;
		}
	}

	void flushClients() {
		lock_guard<mutex> clienListLocker(clientListMtx);
		for (auto iter = clientList.begin(); iter != clientList.end();) {
			if (iter->second->isConnected == false) 
				iter = clientList.unsafe_erase(iter);
			else 
				iter++;
		}
	}
};

class _11_IOCP_채팅서버 {
	static _11_IOCP_채팅서버* instance;
	static thread concurrentIOThreads[MAX_CONCURRENT_IO_THREAD_COUNT];
	static thread messageSenderThread;
	static mutex consoleMtx;

	//서브 쓰레드 : 서버 제어용
	static void MessageSender() {
		int message = 0;
		cin >> message;
		
		if (message == -1) {
			getInstance()->stopServer();
		}
	}

	//서브 쓰레드 : IO처리 담당
	static void IOWorker(HANDLE iocpObject) {
		ClientManager* clientManager = ClientManager::getInstance();
		HANDLE hIocpObject = iocpObject;
		DWORD tranceferredByteCount = 0;
		DWORD completionKey;
		OverlappedIO* overlappedIO;

		while (true) {
			if (GetQueuedCompletionStatus(hIocpObject, &tranceferredByteCount, (PULONG_PTR)&completionKey, (LPOVERLAPPED*)&overlappedIO, WSA_INFINITE)) {
				if (overlappedIO->ioType == IO_RECV) {
					Client* client = (Client*)overlappedIO->data;
					cout << client->socket << " 클라이언트로부터 "<< tranceferredByteCount << "바이트 수신 / 내용 : " << client->recvBuffer << endl;
					clientManager->broadcast(client, client->recvBuffer, strlen(client->recvBuffer) + 1);
					clientManager->postRecv(client);
				}
				else if (overlappedIO->ioType == IO_SEND) {
					Client* client = (Client*)overlappedIO->data;
					lock_guard<mutex> consoleLocker(consoleMtx);
					cout << client->socket << " 클라이언트에게 " << tranceferredByteCount << "바이트 전송" << endl;
				}
				else if (overlappedIO->ioType == IO_STOP) {
					lock_guard<mutex> consoleLocker(consoleMtx);
					cout << this_thread::get_id() << "번 쓰레드가 정지신호를 받음" << endl;
					break;
				}
				else {
					lock_guard<mutex> consoleLocker(consoleMtx);
					cout << this_thread::get_id() << "번 쓰레드가 이상한 데이터 수신 : " << tranceferredByteCount << " / " << completionKey << " / " << overlappedIO->ioType << endl;
				}
			}
			else {
				if (tranceferredByteCount <= 0) {
					Client* client = (Client*)overlappedIO->data;
					cout << client->socket << " 클라이언트와의 연결이 끊어졌습니다" << endl;
					client->isConnected = false;
				}
				else {
					printError("GetQueuedCompletionStatus returend false :", WSAGetLastError());
				}
			}
		}
	}
private:
	volatile bool stopped;
private:
	_11_IOCP_채팅서버() {
		stopped = false;
	}

	void instance_main() {
		WSAData wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			printError("WSAStartup error : ", WSAGetLastError());
			return;
		}

		SOCKET serverSocket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		SOCKADDR_IN serverAddress;
		serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(8500);

		HANDLE hIocpObject = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, MAX_CONCURRENT_IO_THREAD_COUNT);

		for (int i = 0; i < MAX_CONCURRENT_IO_THREAD_COUNT; i++) 
			concurrentIOThreads[i] = thread(IOWorker, hIocpObject);
		

		if (::bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(SOCKADDR)) == SOCKET_ERROR) {
			printError("bind error : ", WSAGetLastError());
			return;
		}

		if (::listen(serverSocket, 5000) == SOCKET_ERROR) {
			printError("listen error : ", WSAGetLastError());
			return;
		}

		DWORD mode = 1;
		::ioctlsocket(serverSocket, FIONBIO, &mode);
		messageSenderThread = thread(MessageSender);

		while (stopped == false) {
			SOCKET clientSocket = ::accept(serverSocket, nullptr, nullptr);
			if (clientSocket == INVALID_SOCKET) {
				if (WSAGetLastError() != WSAEWOULDBLOCK) {
					stopServer();
					printError("accept error : ", WSAGetLastError());
				}
				continue;
			}

			HANDLE iocpHandle = CreateIoCompletionPort((HANDLE)clientSocket, hIocpObject, clientSocket, MAX_CONCURRENT_IO_THREAD_COUNT);
			if (iocpHandle != hIocpObject) {
				printError("CreateIoCompletionPort error : ", WSAGetLastError());
				continue;
			}
			
			cout << clientSocket << " 클라이언트가 서버에 접속하였습니다" << endl;
			Client* client = new Client(clientSocket);
			ClientManager::getInstance()->addClient(client);
			ClientManager::getInstance()->postRecv(client);
			ClientManager::getInstance()->flushClients();
		}

		for (int i = 0; i < MAX_CONCURRENT_IO_THREAD_COUNT; i++) {
			OverlappedIO overlapped;
			overlapped.ioType = IO_STOP;
			if (PostQueuedCompletionStatus(hIocpObject, 1000, 1000, (LPOVERLAPPED)&overlapped) == false) {
				printError("PostQueuedCompletionStatus error : ", WSAGetLastError());
				return;
			}
		}

		for (int i = 0; i < MAX_CONCURRENT_IO_THREAD_COUNT; i++) 
			concurrentIOThreads[i].join();
		messageSenderThread.join();

		cout << "서버를 안전하게 종료하였습니다" << endl;
	}

	void stopServer() {
		stopped = true;
	}
public:
	static void main() {
		_11_IOCP_채팅서버::getInstance()->instance_main();
	}

	static _11_IOCP_채팅서버* getInstance() {
		if (instance == nullptr) {
			instance = new _11_IOCP_채팅서버();
		}
		return instance;
	}
};

_11_IOCP_채팅서버* _11_IOCP_채팅서버::instance = nullptr;
thread _11_IOCP_채팅서버::concurrentIOThreads[MAX_CONCURRENT_IO_THREAD_COUNT];
thread _11_IOCP_채팅서버::messageSenderThread;
mutex _11_IOCP_채팅서버::consoleMtx;
ClientManager* ClientManager::instance = nullptr;