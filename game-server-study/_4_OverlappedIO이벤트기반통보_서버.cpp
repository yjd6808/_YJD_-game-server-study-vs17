//#define _CRT_SECURE_NO_WARNINGS
//
//#include <WinSock2.h>
//#include <Ws2tcpip.h>
//#include <iostream>
//#include <thread>
//#include <vector>
//
//using namespace std;
//
//#define BUF_SIZE 1024
//
//struct Client {
//	WSAOVERLAPPED clientOverlapped;
//	SOCKET clientSocket;
//	WSABUF clientRecvBuf;
//	WSAEVENT recvEventObj;
//	DWORD recvflag;
//	DWORD recvBytesCount;
//	DWORD recvLastErrorID;
//	char clientRecvBuffer[BUF_SIZE];
//
//	Client(SOCKET socket) {
//		clientSocket = socket;
//		memset(&clientOverlapped, 0, sizeof(clientOverlapped));
//		recvEventObj = WSACreateEvent();
//		clientOverlapped.hEvent = recvEventObj;
//		clientRecvBuf.buf = clientRecvBuffer;
//		clientRecvBuf.len = BUF_SIZE;
//		recvBytesCount = 0;
//		recvflag = 0;
//	}
//};
//
//std::string GetLastErrorAsString(DWORD id)
//{
//	//Get the error message, if any.
//	DWORD errorMessageID = id;
//	if (errorMessageID == 0)
//		return std::string(); //No error message has been recorded
//
//	LPSTR messageBuffer = nullptr;
//	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
//
//	std::string message(messageBuffer, size);
//
//	//Free the buffer.
//	LocalFree(messageBuffer);
//
//	return message;
//}
//
//#define BUF_SIZE 1024
//
//void ErrorHandling(const char* message);
//
//vector<Client*> clientList;
//vector<thread> clientThread;
//
//int main(int argc, char* argv[])
//{
//	WSADATA wsaData;
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		ErrorHandling("WSAStartup() error!");
//		return -1;
//	}
//
//	SOCKET serverSocket = WSASocket(PF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
//	SOCKADDR_IN serverAddress;
//	serverAddress.sin_port = htons(8500);
//	serverAddress.sin_family = AF_INET;
//	inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
//
//	if (::bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
//		cout << "bind error" << endl;
//		exit(-1);
//	}
//
//	if (::listen(serverSocket, 5000) == SOCKET_ERROR) {
//		cout << "listen error" << endl;
//		exit(-1);
//	}
//
//	clientList.reserve(sizeof(Client) * 100);
//	
//	while (1) {
//		SOCKADDR_IN clientAddress;
//		int clientAddressSize = sizeof(clientAddress);
//		SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &clientAddressSize);
//		Client* client = new Client(clientSocket);
//		clientList.push_back(client);
//
//		WSARecv(clientSocket, &client->clientRecvBuf, 1, &client->recvBytesCount, &client->recvflag, &client->clientOverlapped, nullptr);
//		client->recvLastErrorID = WSAGetLastError();
//		if (client->recvLastErrorID != ERROR_IO_PENDING) {
//			cout << "클라이언트 : " << client->clientSocket << " / WSARecv 에러 발생" << endl << "사유 : " << GetLastErrorAsString(client->recvLastErrorID).c_str() << endl;
//			continue;
//		}
//
//		//각각의 클라이언트마다 쓰레드 생성해줌
//		clientThread.emplace_back([client]
//		{
//			while (1) {
//				if (client->recvLastErrorID == ERROR_IO_PENDING) {
//					cout << client->clientSocket << " 클라이언트 수신 대기중" << endl;
//					WSAWaitForMultipleEvents(1, &client->recvEventObj, true, WSA_INFINITE, false);
//					WSAGetOverlappedResult(client->clientSocket, &client->clientOverlapped, &client->recvBytesCount, false, &client->recvflag);
//
//					if (client->recvBytesCount <= 0) {
//						cout << client->clientSocket << " 클라이언트와의 접속이 끊어졌습니다." << endl;
//						break;
//					}
//					cout << "수신데이터 : " << client->clientRecvBuffer << endl;
//					WSARecv(client->clientSocket, &client->clientRecvBuf, 1, &client->recvBytesCount, &client->recvflag, &client->clientOverlapped, nullptr);
//					client->recvLastErrorID = WSAGetLastError();
//				}
//			}
//		});
//	}
//
//	//정리작업 해줘야함
//	//IO 종료를 기다림
//	//IO 완료된 클라이언트들 이벤트 해제 및 소켓 닫기
//	//WSA dll 사용종료를 알림 / WSACleanUp
//
//	return 0;
//}
//
//
//
//void ErrorHandling(const char* message)
//{
//	fputs(message, stderr);
//	fputc('\n', stderr);
//	exit(1);
//}
//
