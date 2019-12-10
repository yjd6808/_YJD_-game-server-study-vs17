//#include <Windows.h>
//#include <thread>
//#include <iostream>
//#include <vector>
//#include <mutex>
//
//using namespace std;
//
//class SemaPhore {
//private:
//	mutex mtx;
//	condition_variable cv;
//	int count;
//
//public:
//	SemaPhore(int sharedThreadCount) {
//		count = sharedThreadCount;
//	}
//
//	void Wait() {
//		unique_lock<mutex> locker(mtx);
//		cv.wait(locker, [&] {return count > 0; });
//		count--;
//	}
//
//	void Release() {
//		unique_lock<mutex> locker(mtx);
//		count++;
//	}
//};
//
//
//int sharedResource = 0;
//int sharedResource2 = 0;
//SemaPhore srSemaPhore(2);
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
//	for (int i = 0; i < 5; i++) {
//		threadVec.emplace_back([] 
//		{
//			while (1) {
//				srSemaPhore.Wait();
//				job(&sharedResource);
//				job(&sharedResource);
//				job(&sharedResource);
//				job(&sharedResource);
//				job(&sharedResource);
//				srSemaPhore.Release();
//			}
//		});
//	}
//
//	while (1); //끝나지마라 메인쓰레드야
//
//	return 0;
//}
