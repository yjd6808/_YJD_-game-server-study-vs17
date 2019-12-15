#pragma once


#include "common_header.h"
#include "Socket.h"

using namespace std;

class _6_OverlappedIO_Callback����뺸_���� {
	struct Client {
		WSAOVERLAPPED overlapped;
		SOCKET socket;
		WSABUF buf;
		
		char buffer[BUF_SIZE];
		char address[20];

		DWORD flag;
		DWORD transferedByteCount;
		DWORD lastErrorID;

		Client(SOCKET socket) {
			this->socket = socket;
			ZeroMemory(&overlapped, sizeof(WSAOVERLAPPED));
			buf.buf = buffer;
			buf.len = BUF_SIZE;
			transferedByteCount = 0;
			flag = 0;
			lastErrorID = 0;
		}

		void initOverlapped() {
			ZeroMemory(&overlapped, sizeof(WSAOVERLAPPED));
			buf.buf = buffer;
			buf.len = BUF_SIZE;
			transferedByteCount = 0;
			flag = 0;
			lastErrorID = 0;
		}
	};

	vector<Client*> clients;


	static void  CALLBACK CompRoutine(DWORD error, DWORD transferredByteCounts, LPWSAOVERLAPPED lpOverlapped, DWORD flag) {
		Client* client = (Client*)lpOverlapped;
		if (transferredByteCounts <= 0) {
			cout << this_thread::get_id() << " " << client->socket << "���� ������ ���������ϴ�" << endl;
			//Ŭ������ �̰� �����Ƽ� ������
			return;
		}

		cout << client->buffer << endl;
		WSARecv(client->socket, &client->buf, 1, &client->transferedByteCount, &client->flag, &client->overlapped, CompRoutine);
		client->lastErrorID = WSAGetLastError();
		if (client->lastErrorID != WSA_IO_PENDING) {
			cout << "RecvError Occurred : " << GetLastErrorAsString(client->lastErrorID) << endl;
			exit(-1);
		}
	}

public :
	void instance_main() {
		ServerSocket serverSocket(TransportLayerProtocol::TCP);
		serverSocket.Bind("127.0.0.1", 8500);
		serverSocket.Listen();

		while (1) {
			
			SOCKET clientSocket = accept(serverSocket.Get(), nullptr, nullptr);
			cout << this_thread::get_id() << " Ŭ���̾�Ʈ " << clientSocket << "�� �����߽��ϴ�" << endl;

			Client* client = new Client(clientSocket);
			clients.push_back(client);
			

			WSARecv(client->socket, &client->buf, 1, &client->transferedByteCount, &client->flag, &client->overlapped, CompRoutine);
			client->lastErrorID = WSAGetLastError();
			if (client->lastErrorID != WSA_IO_PENDING) {
				cout << "RecvError Occurred : " << GetLastErrorAsString(client->lastErrorID) << endl;
				exit(-1);
			}
		}
	}

	static void main() {
		_6_OverlappedIO_Callback����뺸_���� instance;
		instance.instance_main();
	}
};
