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
////Ŭ���̾�Ʈ �Ϲ� ����ŷ���� ����� �ٷ��
////���� ������ ������� ���������� �ҋ� ���������� ����»��¸� Ȯ��  �Ұ��̱� ������ CPU�� 100%Ȱ���ϰԵȴ�.
//
////select�� �Ἥ ������Լ� ȣ�� �� ������� �ٽ� ���� �ʿ� ���� �ش� ������� �Ϸ�� ���ϸ� �����ͼ� ó���Ѵ�.
//
////Overlapped IO�� 
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
//		cout << "������Դϴ�" << endl;
//
//		SOCKET client = accept(Socket, nullptr, 0);
//
//		cout << client <<  "����" << endl;
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
