/*
 *@Author		: JungDo Yun
 *@Created		: $time$
 *@Description	:
 */

#pragma once

#include "common_header.h"
#include "Socket.h"

using namespace std;

class _7_OverlappedIO_CallBack기반통보_클라 {

	//완료 루틴 양식
	/*(CALLBACK * LPWSAOVERLAPPED_COMPLETION_ROUTINE)(
		IN DWORD dwError,
		IN DWORD cbTransferred,
		IN LPWSAOVERLAPPED lpOverlapped,
		IN DWORD dwFlags
		);*/
	static void CALLBACK SendCompRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {
		if (dwError != 0) {
			cout << "send completion routine error : " << GetLastErrorAsString(dwError);
			exit(-1);
		}

		cout << cbTransferred << "바이트 메세지 전송완료" << endl;

		OverlappedIOContext* sendContext = (OverlappedIOContext*)lpOverlapped;
		SOCKET clientSocket = *((SOCKET*)sendContext->data);
		delete sendContext;

		OverlappedIOContext* recvContext = new OverlappedIOContext(IO_RECV);
		recvContext->SetRecvBuffer();
		
		DWORD flag = 0;
		DWORD recvByteCount = 0;
		DWORD errorID = 0;

		WSARecv(clientSocket, &recvContext->wsaBuf, 1, &recvByteCount, &flag, (OVERLAPPED*)recvContext, RecvCompRoutine);
		errorID = WSAGetLastError();
		if (errorID != 0 && errorID != WSA_IO_PENDING) {
			cout << "전송오류 : " << GetLastErrorAsString(errorID) << endl;
			exit(-1);
		}
		SleepEx(INFINITE, true);
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
		cout << cbTransferred << "바이트 메세지 수신완료" << endl;
		cout << "내용 : " << recvContext->buffer << endl;

		delete recvContext;
	}
private:
	void instance_main() {
		ClientSocket socket(TransportLayerProtocol::TCP);
		socket.Connect("127.0.0.1", 8500);

		while (true) {
			
			char message[100];
			cout << "메시지를 입력해주세요 : ";
			cin >> message;

			OverlappedIOContext* sendContext = new OverlappedIOContext(IO_SEND);
			sendContext->SetSendBuffer(message, strlen(message) + 1);
			sendContext->data = &socket;
			DWORD byteCount;
			DWORD errorID;
			

			WSASend(socket.Get(), &sendContext->wsaBuf, 1, &byteCount, 0, (OVERLAPPED*)sendContext, SendCompRoutine);
			errorID = WSAGetLastError();

			if (errorID != 0 && errorID != WSA_IO_PENDING) {
				cout << "전송오류 : " << GetLastErrorAsString(errorID) << endl;
				exit(-1);
			}

			SleepEx(INFINITE, true);
		}
	}
public:
	static void main() {
		_7_OverlappedIO_CallBack기반통보_클라 instance;
		instance.instance_main();
	}
};