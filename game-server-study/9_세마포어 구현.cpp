//#include <Windows.h>
//#include <thread>
//#include <iostream>
//#include <vector>
//#include <mutex>
//
//using namespace std;
//
//class Semaphore {
//private:
//	mutex mtx;
//	condition_variable cv;
//	int count;
//
//public:
//	Semaphore(int sharedThreadCount) {
//		count = sharedThreadCount;
//	}
//
//	inline void Wait() {
//		unique_lock<mutex> locker(mtx);
//		cv.wait(locker, [&] {return count > 0; });
//		count--;
//	}
//
//	inline void Notify() {
//		unique_lock<mutex> locker(mtx);
//		cv.notify_one();
//		count++;
//	}
//};
//
//
//
//int sharedResource = 0;
//Semaphore srSemaPhore(2);
//int currentUsingThreadCount = 0;
//std::mutex mtx;
//
//condition_variable fff;
//
//void job(int* resource) {
//	for (int i = 0; i < 50000000; i++)
//		(*resource)++;
//	cout << this_thread::get_id() << " : " << *resource << endl;
//}
//
//int main() {
//	vector<thread> threadVec;
//
//	sharedResource = 100;
//	thread tt([] 
//	{
//		while (1) {
//			std::mutex mx;
//			unique_lock<mutex> locker(mx);
//			fff.wait(locker, [] { return  sharedResource > 0; });
//			cout << "fesfesef";
//			fff.notify_all();
//			Sleep(1000);
//		}
//	});
//
//	tt.join();
//
//	for (int i = 0; i < 5; i++) {
//		threadVec.emplace_back([] 
//		{
//			srSemaPhore.Wait();
//			cout << "왜 동작하지.." << endl;
//			srSemaPhore.Notify();
//			//srSemaPhore.Notify();
//			/*while (1) {
//				srSemaPhore.Wait();
//				mtx.lock();
//				currentUsingThreadCount++;
//				mtx.unlock();
//				job(&sharedResource);
//				job(&sharedResource);
//				job(&sharedResource);
//				job(&sharedResource);
//				job(&sharedResource);
//				srSemaPhore.Notify();
//				mtx.lock();
//				currentUsingThreadCount--;
//				mtx.unlock();
//			}*/
//		});
//	}
//
//	thread monitorthread([] 
//	{
//		while (1) {
//			Sleep(1000);
//			cout << "현재 작업중인 쓰레드 수 : " << currentUsingThreadCount << endl;
//		}
//	});
//
//	while (1); //끝나지마라 메인쓰레드야
//
//	return 0;
//}
