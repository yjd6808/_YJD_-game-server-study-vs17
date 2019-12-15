//#include <WinSock2.h>
//#include <iostream>
//#include <vector>
//#include <Ws2tcpip.h>
//
//using namespace std;
//
//sockaddr_in&& SetupEndPoint(const char* addr, const int port);
//std::string GetLastErrorAsString();
//
////클라이언트 일반 논블로킹으로 입출력 다루기
////논블록 상태의 입출력을 지속적으로 할떄 지속적으로 입출력상태를 확인  할것이기 떄문에 CPU를 100%활용하게된다.
//
////select를 써서 입출력함수 호출 후 입출력이 다시 해줄 필요 없이 해당 입출력이 완료된 소켓만 가져와서 처리한다.
//
////Overlapped IO를 
//
//
//
//int main() {
//	try
//	{
//		WSADATA w;
//		WSAStartup(MAKEWORD(2, 2), &w);
//
//		SOCKET Socket = WSASocket(PF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
//		sockaddr_in tcpServerAddr = SetupEndPoint("127.0.0.1", 5782);
//		if (bind(Socket, (sockaddr*)&tcpServerAddr, sizeof(tcpServerAddr)) < 0)
//		{
//			cout << "bind failed:" << GetLastErrorAsString().c_str() << endl;
//			throw exception(GetLastErrorAsString().c_str());
//		}
//
//		listen(Socket, 5000);
//
//		//sockaddr_in addr;
//		//int len;
//		cout << "대기중입니다" << endl;
//
//		SOCKET client = accept(Socket, nullptr, 0);
//
//		cout << client <<  "접속" << endl;
//
//		while (1);
//	}
//	catch (std::exception e) {
//		cout << e.what() << endl;
//	}
//		
//
//
//	WSACleanup();
//	
//	return 0;
//}
//
//sockaddr_in&& SetupEndPoint(const char* addr, const int port)
//{
//	sockaddr_in tcpServerAddr;
//	memset(&tcpServerAddr, 0, sizeof(tcpServerAddr));
//	tcpServerAddr.sin_family = AF_INET;
//	inet_pton(AF_INET, addr, &tcpServerAddr.sin_addr);
//	tcpServerAddr.sin_port = htons((uint16_t)port);
//	return std::move(tcpServerAddr);
//}
//
//
//
//std::string GetLastErrorAsString()
//{
//#ifdef _WIN32
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
//#else 
//	std::string message = strerror(errno);
//#endif
//	return message;
//}
