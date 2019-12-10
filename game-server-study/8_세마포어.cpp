//#include <Windows.h>
//#include <thread>
//#include <iostream>
//#include <vector>
//#include <mutex>
//
//using namespace std;
//
//HANDLE semaPhore;
//int sharedResource = 0;
//int currentUsingThreadCount = 0;
//std::mutex mtx;
//
//void job(int* resource) {
//	for (int i = 0; i < 50000000; i++)
//		(*resource)++;
//	cout << this_thread::get_id() << " : " << "�۾� ��" << endl;
//}
//
//
//int main() {
//	vector<thread> threadVec;
//	semaPhore = CreateSemaphore(nullptr, 2, 2, "");
//
//	for (int i = 0; i < 5; i++) {
//		threadVec.emplace_back([]
//		{
//			while (1) {
//				WaitForSingleObject(semaPhore, INFINITE);
//				mtx.lock();
//				currentUsingThreadCount++;
//				mtx.unlock();
//				job(&sharedResource);
//				job(&sharedResource);
//				job(&sharedResource);
//				job(&sharedResource);
//				job(&sharedResource);
//				ReleaseSemaphore(semaPhore, 1, nullptr);
//				mtx.lock();
//				currentUsingThreadCount--;
//				mtx.unlock();
//			}
//		});
//	}
//
//	thread monitorThread([] 
//	{
//		while (1) {
//			Sleep(1000);
//			cout << "���� �۾����� ������ �� : " << currentUsingThreadCount << endl;
//		}
//	});
//
//	while (1); //���������� ���ξ������
//
//	CloseHandle(semaPhore);
//
//	return 0;
//}
