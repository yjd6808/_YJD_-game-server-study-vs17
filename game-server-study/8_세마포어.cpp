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
//	cout << this_thread::get_id() << " : " << "작업 중" << endl;
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
//			cout << "현재 작업중인 쓰레드 수 : " << currentUsingThreadCount << endl;
//		}
//	});
//
//	while (1); //끝나지마라 메인쓰레드야
//
//	CloseHandle(semaPhore);
//
//	return 0;
//}
