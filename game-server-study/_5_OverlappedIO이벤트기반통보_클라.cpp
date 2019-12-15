//#define _CRT_SECURE_NO_WARNINGS
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//
//#include <WinSock2.h>
//#include <Ws2tcpip.h>
//#include <iostream>
//#include <thread>
//
//#define BUF_SIZE 512
//using namespace std;
////Returns the last Win32 error, in string format. Returns an empty string if there is no error.
//std::string GetLastErrorAsString()
//{
//	//Get the error message, if any.
//	DWORD errorMessageID = ::GetLastError();
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
//int main()
//{
//	WSADATA wsaData;
//	WSAStartup(MAKEWORD(2, 2), &wsaData);
//
//	SOCKET clientSocket = WSASocket(PF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
//	SOCKADDR_IN serverAddress;
//	serverAddress.sin_port = htons(8500);
//	serverAddress.sin_family = AF_INET;
//	inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
//
//	if (::connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
//		cout << "connect error" << endl;
//		exit(-1);
//	}
//
//	WSAEVENT eventObj = WSACreateEvent();
//	WSAOVERLAPPED wsaOverlapped;
//	ZeroMemory(&wsaOverlapped, sizeof(wsaOverlapped));
//
//	wsaOverlapped.hEvent = eventObj;
//
//	char sendBuffer[BUF_SIZE];
//
//	WSABUF sendBuf;
//	sendBuf.buf = sendBuffer;
//	sendBuf.len = BUF_SIZE;
//	DWORD flag;
//	
//	DWORD sendBytes;
//
//	while (1) {
//		cout << "보낼 메세지 입력 : ";
//		scanf("%s", sendBuffer);
//
//		if (WSASend(clientSocket, &sendBuf, 1, &sendBytes, 0, &wsaOverlapped, nullptr) == SOCKET_ERROR) {
//			if (WSAGetLastError() == ERROR_IO_PENDING) {
//				
//				WSAWaitForMultipleEvents(1, &wsaOverlapped.hEvent, true, WSA_INFINITE, false);
//				WSAGetOverlappedResult(clientSocket, &wsaOverlapped, &sendBytes, true, &flag);
//
//				cout << "메세지 전송완료" << endl;
//			}
//		}
//	}
//
//
//
//	return 0;
//}
//
//
//
//
//
