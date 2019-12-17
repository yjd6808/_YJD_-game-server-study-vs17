/*
 *@Author		: JungDo Yun
 *@Created		: 2019-12-16 오후 5:36:35
 *@Description	:
 */
#include <concurrent_queue.h>
#include "common_header.h"
#include "Socket.h"

using namespace std;

struct Message {
	char message[100];
};

//동적할당된 메모리는 따로 해제 안함 구현에만 목적을 둔 코드이다.
class _10_OverlappedIO_CallBack기반통보_채팅클라 {
	static _10_OverlappedIO_CallBack기반통보_채팅클라* instance;
	concurrency::concurrent_queue<Message> messageQueue;
	ClientSocket* clientSocket;
private:
	static void postRecv() {

	}

	static void postSend() {

	}

	static void CALLBACK SendCompRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {
		if (dwError != 0) {
			cout << "send completion routine error : " << GetLastErrorAsString(dwError);
			exit(-1);
		}

		cout << cbTransferred << "바이트 메세지 전송완료" << endl;

		OverlappedIOContext* sendContext = (OverlappedIOContext*)lpOverlapped;
		SOCKET clientSocket = *((SOCKET*)sendContext->data);
		delete sendContext;
	}

	static void CALLBACK RecvCompRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {
		if (dwError != 0) {
			cout << "send completion routine error : " << GetLastErrorAsString(dwError);
			exit(-1);
		}

		if (cbTransferred <= 0) {
			cout << "서버와의 연결이 끊어졌습니다" << endl;
			exit(-1);
		}

		OverlappedIOContext* recvContext = (OverlappedIOContext*)lpOverlapped;
		SOCKET clientSocket = *((SOCKET*)recvContext->data);
		cout << cbTransferred << "바이트 메세지 수신완료" << endl;
		cout << "내용 : " << recvContext->buffer << endl;

		DWORD flag = 0;
		DWORD recvByteCount = 0;
		DWORD errorID = 0;

		recvContext->SetRecvBuffer();
		WSARecv(clientSocket, &recvContext->wsaBuf, 1, &recvByteCount, &flag, (OVERLAPPED*)recvContext, RecvCompRoutine);
		errorID = WSAGetLastError();
		if (errorID != 0 && errorID != WSA_IO_PENDING) {
			cout << "전송오류 : " << GetLastErrorAsString(errorID) << endl;
			exit(-1);
		}
	}
private:
	void instance_main() {
		clientSocket = new  ClientSocket(TransportLayerProtocol::TCP);
		clientSocket->Connect("127.0.0.1", 8500);

		thread ioHandlingThread(&_10_OverlappedIO_CallBack기반통보_채팅클라::ioHandlingThreadMain, this);

		//보낼 메세지를 thread-safe-queue에 담는다.
		//이런식으로 만든 이유가 키보드 입력을 하려고 while문 내부에서 블로킹이 걸리게된다. 
		//그래서 SleepEx 호출이안되기 때문에 완료루틴 호출이 안됨
		while (true) {
			Message sMessage;
			cin >> sMessage.message;
			messageQueue.push(sMessage);

		}
	}

	//IO처리를 담당할 쓰레드(서브 쓰레드)
	void ioHandlingThreadMain() {
		ClientSocket* socket = _10_OverlappedIO_CallBack기반통보_채팅클라::getInstance()->clientSocket;


		OverlappedIOContext* recvContext = new OverlappedIOContext(IO_RECV);
		recvContext->data = socket;
		recvContext->SetRecvBuffer();

		DWORD recvByteCount = 0;
		DWORD flag = 0;

		if (WSARecv(socket->Get(), &recvContext->wsaBuf, 1, &recvByteCount, &flag, (OVERLAPPED*)recvContext, RecvCompRoutine) == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				cout << "수신오류 : " << GetLastErrorAsString(WSAGetLastError()) << endl;
				exit(-1);
			}
		}

		while (true) {
			Message message;
			if (messageQueue.try_pop(message)) {
				OverlappedIOContext* sendContext = new OverlappedIOContext(IO_SEND);
				sendContext->SetSendBuffer(message.message, strlen(message.message) + 1);
				sendContext->data = socket;

				DWORD byteCount;

				if (WSASend(socket->Get(), &sendContext->wsaBuf, 1, &byteCount, 0, (OVERLAPPED*)sendContext, SendCompRoutine) == SOCKET_ERROR) {
					if (WSAGetLastError() != WSA_IO_PENDING) {
						cout << "전송오류 : " << GetLastErrorAsString(WSAGetLastError()) << endl;
						exit(-1);
					}
				}
			}

			SleepEx(1, true);
		}
	}

public:
	static void main() {
		_10_OverlappedIO_CallBack기반통보_채팅클라::getInstance()->instance_main();
	}

	static _10_OverlappedIO_CallBack기반통보_채팅클라* getInstance() {
		if (instance == nullptr) {
			instance = new _10_OverlappedIO_CallBack기반통보_채팅클라();
		}
		return instance;
	}
};

_10_OverlappedIO_CallBack기반통보_채팅클라* _10_OverlappedIO_CallBack기반통보_채팅클라::instance = nullptr;
