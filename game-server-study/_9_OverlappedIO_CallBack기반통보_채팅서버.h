#include <stdio.h>
/*
 *@Author		: JungDo Yun
 *@Created		: 2019-12-16 오후 12:55:59
 *@Description	: 채팅서버 / Accept -> 메인쓰레드, IO -> 서브쓰레드
				: IOCP 들어가기전에 철저하게 그리고 오류없이 킹오브 주석처리로 만들어보자
 */

#pragma once

#include "common_header.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <concurrent_queue.h>

using namespace std;

#define BUF_SIZE 1024



enum IOType {
	IO_RECV,
	IO_SEND
};


//중첩 IO에서 함께 전달할 데이터를 묶음
struct OverlappedIOData {
	OVERLAPPED overlapped;
	WSABUF buf;
	void* data;

	OverlappedIOData(IOType ioType) {
		ZeroMemory(&overlapped, sizeof(overlapped));
		data = nullptr;
	}

	void setBuffer(char* buffer, const int len) {
		buf.buf = buffer;
		buf.len = len;
	}
};

class Client {

private:
	SOCKET socket;
	char sendBuffer[BUF_SIZE];
	int sendBufferLen;

	char recvBuffer[BUF_SIZE];
	bool isConnected;
	
	
	

public:
	Client(SOCKET clientSocket) {
		socket = clientSocket;
		sendBufferLen = 0;
		isConnected = true;
		ZeroMemory(sendBuffer, BUF_SIZE);
		ZeroMemory(recvBuffer, BUF_SIZE);
	}

	char* getRecvBuffer() { return recvBuffer; }
	char* getSendBuffer() { return sendBuffer; }
	int getSendBufferLen() { return sendBufferLen; }
	void setSendBuffer(char* data, int len) {
		memcpy(sendBuffer, data, len);
		sendBufferLen = len;
	}


	bool connected() { return isConnected; }

	void disconnect() {
		isConnected = false;
	}

	void clearSendBuffer() {
		ZeroMemory(sendBuffer, sizeof(BUF_SIZE));
		sendBufferLen = 0;
	}

	void clearRecvBuffer() {
		ZeroMemory(recvBuffer, sizeof(BUF_SIZE));
	}

	SOCKET getSocket() { return socket; }

	
};

class _9_OverlappedIO_CallBack기반통보_채팅서버 {
	static _9_OverlappedIO_CallBack기반통보_채팅서버* instance;
private:
	mutex clientListMutex;
	unordered_map<SOCKET, Client*> clientList;
	concurrency::concurrent_queue<SOCKET>  acceptClientQueue;
	atomic<bool> stopWorking;
private:
	//이거 foward declarations(전방 선언)으로 해서 Client가 부르게 하고싶은데 자꾸 오류터짐

	//class _9_OverlappedIO_CallBack기반통보_채팅서버;

	//이렇게 선언해도 왜 못찾는다고 뜨는거지 ㄷㄷ;
	//고민해보자
	//차선책으로 그냥 인자받아서 넣어버림.. OOP 갓다버렸다.
	//물론 다른 헤더파일에 Client클래스 만들면 되는데 하나의 헤더파일에서 모두 구현해버리고 싶어서;
	static void postRecv(Client* client) {
		OverlappedIOData* recvOverlappedIOData = new OverlappedIOData(IO_RECV); 
		recvOverlappedIOData->setBuffer(client->getRecvBuffer(), BUF_SIZE);
		recvOverlappedIOData->data = client;

		DWORD recvedByteCount = 0;
		DWORD flag = 0;

		//수신대기
		if (WSARecv(client->getSocket(), &recvOverlappedIOData->buf, 1, &recvedByteCount, &flag, (OVERLAPPED*)recvOverlappedIOData, _9_OverlappedIO_CallBack기반통보_채팅서버::RecvCompletionRoutine) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING)
				client->disconnect();
		}
	}

	static void postSend(Client* client) {
		OverlappedIOData* sendOverlappedIOData = new OverlappedIOData(IO_SEND);
		sendOverlappedIOData->setBuffer(client->getSendBuffer(), client->getSendBufferLen());
		sendOverlappedIOData->data = client;

		DWORD sendByteCount = 0;

		//송신
		if (WSASend(client->getSocket(), &sendOverlappedIOData->buf, 1, &sendByteCount, 0, (OVERLAPPED*)sendOverlappedIOData, _9_OverlappedIO_CallBack기반통보_채팅서버::SendCompletionRoutine) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING)
				client->disconnect();
		}
	}
public:
	static void CALLBACK RecvCompletionRoutine(DWORD dwError, DWORD recvByteCount, OVERLAPPED* lpOverlapped, DWORD flag) {
		OverlappedIOData* overlappedData = (OverlappedIOData*)lpOverlapped;
		Client* client = (Client*)overlappedData->data;

		//연결이 끊긴 상태라면 진행안함
		if (client->connected() == false)
			return;

		//오류 발생 또는 수신 바이트 크기가 0이라면 연결을 끊어버림
		if (dwError != 0 || recvByteCount == 0)
		{
			client->disconnect();
			return;
		}

		cout << client->getSocket() << "로부터 " << recvByteCount << "바이트 데이터 수신 / 내용 : " << client->getRecvBuffer() << endl;


		//보낸 사람을 제외한 다른 클라이언트 들에게 전송해줘야한다.
		for (auto iter = _9_OverlappedIO_CallBack기반통보_채팅서버::getInstance()->clientList.begin(); 
			      iter != _9_OverlappedIO_CallBack기반통보_채팅서버::getInstance()->clientList.end(); 
			      iter++) {
			Client* otherClient = iter->second;

			if (client == otherClient)
				continue;

			otherClient->setSendBuffer(client->getRecvBuffer(), recvByteCount);
		}

		//IO 정리
		delete overlappedData;
		
		//수신 대기상태로 둔다.
		postRecv(client);
	}

	static void CALLBACK SendCompletionRoutine(DWORD dwError, DWORD sendByteCount, OVERLAPPED* lpOverlapped, DWORD flag) {
		OverlappedIOData* overlappedData = (OverlappedIOData*)lpOverlapped;
		Client* client = (Client*)overlappedData->data;

		//연결이 끊긴 상태라면 진행안함
		if (client->connected() == false)
			return;

		//오류 발생 또는 수신 바이트 크기가 0이라면 연결을 끊어버림
		if (dwError != 0 || sendByteCount == 0)
		{
			client->disconnect();
			return;
		}

		//IO 정리
		delete overlappedData;

		//송신 버퍼를 비워줌
		client->clearSendBuffer();
	}

private:
	

	//에러 출력함수
	void printError(const char* title, DWORD errorId) {
		cout << title << GetLastErrorAsString(errorId) << endl;
	}

	//프로그램 시작지점이다.
	void instance_main() {

		WSADATA wsaData;

		//윈속 DLL 초기화
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			DWORD errorID = WSAGetLastError();
			printError("WSAStartUp Error : ", errorID);
		}

		//TCP 소켓 생성
		SOCKET serverSocket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);

		//바인드할 주소 초기화
		SOCKADDR_IN serverAddr;
		ZeroMemory(&serverAddr, sizeof(SOCKADDR_IN));

		//항상 AF_INET을 설정한다
		serverAddr.sin_family = AF_INET;

		//htons는 받은 인자값을 호스트 바이트순서(내 컴퓨터는 Intel이니까 Little Endian이다.)에서 
		//네트워크 바이트 순서(Big Endian)로 바꿔준다.
		serverAddr.sin_port = htons(8500); 

		//SOCKADDR과 크기를 맞춰줄려고 넣은 패딩 바이트로 쓸일이 없다.
		serverAddr.sin_zero;

		//주소를 넣어주고 있다.
		//sin_addr은 내부적으로 공용체를 쓰기 때문에 S_un.S_addr필드만 채워주면된다.
		//inet_addr함수는 문자열을 받아 이를 네트워크 바이트 순서를 가진 unsigned long형으로 반환해준다.
		serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

		//true 설정시 nagle 알고리즘을 사용하지 않겠다.
		int option = TRUE;               //네이글 알고리즘 on/off
		setsockopt(serverSocket,             //해당 소켓
			IPPROTO_TCP,          //소켓의 레벨
			TCP_NODELAY,          //설정 옵션
			(const char*)&option, // 옵션 포인터
			sizeof(option));      //옵션 크기



		//비동기 소켓으로 만들기
		//ioctlMode == 0 -> 동기
		//ioctlMode != 0 -> 비동기
		//이번예제에서는 IO처리를 따로 서브쓰레드를 만들어서 할것이기 때문에 동기모드로 한다.
		int ioctlMode = false;
		ioctlsocket(serverSocket, FIONBIO, (u_long*)&ioctlMode);

		//소켓에 서버의 주소를 할당해준다.
		if (::bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
			printError("bind error : ", WSAGetLastError());
			return;
		}

		//연결요청 소켓(클라이언트)가 대기하는 연결 대기열을 생성한다. 
		if (::listen(serverSocket, 5000) == SOCKET_ERROR) {
			printError("listen error : ", WSAGetLastError());
			return;
		}

		



		//IO처리 쓰레드 생성
		thread ioHandlingThread(&_9_OverlappedIO_CallBack기반통보_채팅서버::ioHandlingThreadMain, this);

		//Accept를 담당할 쓰레드(메인 쓰레드)
		while (stopWorking == false) {
			SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
			acceptClientQueue.push(clientSocket);
		}

		ioHandlingThread.join();
	}

	//IO처리를 담당할 쓰레드(서브 쓰레드)
	void ioHandlingThreadMain() {
		while (stopWorking == false) {
			SOCKET clientSocket = INVALID_SOCKET;
			if (acceptClientQueue.try_pop(clientSocket)) {
				//클라이언트 정보 등록
				Client* client = new Client(clientSocket);
				clientList.insert(pair<SOCKET, Client*>(clientSocket, client));

				cout << clientSocket << " 클라이언트가 서버에 접속하였습니다" << endl;

				postRecv(client);
				continue;
			}
			
			//클라이언트들의 송신 버퍼들을 모두 비운다.
			for (auto& clientPair : clientList) {
				Client* client = clientPair.second;

				//연결이 끊긴 소켓들은 작업안함
				if (client->connected() == false)
					continue;

				//송신 버퍼가 비어있는 클라이언트는 작업안함
				if (client->getSendBufferLen() == 0)
					continue;

				postSend(client);
			}

			//연결이 종료된 클라이언트들을 리스트에서 제거한다.
			for (auto iter = clientList.begin(); iter != clientList.end();) {
				if (iter->second->connected() == false) {
					Client* client = iter->second;
					cout << client->getSocket() << " 클라이언트와의 연결이 끊어졌습니다" << endl;
					closesocket(client->getSocket());
					iter = clientList.erase(iter);
					delete client;
				}
				else
					iter++;
			}

			//완료 루틴을 실행한다.
			SleepEx(1, true);
		}
	}
public:
	_9_OverlappedIO_CallBack기반통보_채팅서버() {
		stopWorking = false;

	}
	
	static void main() {
		_9_OverlappedIO_CallBack기반통보_채팅서버::getInstance()->instance_main();
	}

	static _9_OverlappedIO_CallBack기반통보_채팅서버* getInstance() {
		if (instance == nullptr) {
			instance = new _9_OverlappedIO_CallBack기반통보_채팅서버();
		}
		return instance;
	}
};

_9_OverlappedIO_CallBack기반통보_채팅서버* _9_OverlappedIO_CallBack기반통보_채팅서버::instance = nullptr;

