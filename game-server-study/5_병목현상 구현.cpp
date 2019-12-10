//#include "Clock.h"
//#include <iostream>
//#include <mutex>
//#include <thread>
//#include <vector>
//#include <Windows.h>
//
//#define MAX_NUM 50000000
//using namespace std;
//
//void job(int& data) {
//	for (int i = 0; i < MAX_NUM; i++)
//		data += 1;
//	cout << "current thread id " << this_thread::get_id() << " : " << data << endl;
//}
//
//recursive_mutex mtx;
//recursive_mutex mtx2;
//int source_20억카운트 = 0;
//int source2_20억카운트 = 0;
//
//int main() {
//	cout << GetCurrentProcessId() << endl;
//	int source = 0;
//	int source2 = 0;
//	
//	vector<thread> threadVec;
//
//	Clock::Start();
//	
//	//1번쓰레드
//	threadVec.emplace_back([&source, &source2]
//	{
//		while (true)
//		{
//			{
//				lock_guard<recursive_mutex> locker(mtx);
//				job(source);
//
//				if (source >= 1000000000)
//					source = 0;
//			}
//
//			{
//				lock_guard<recursive_mutex> locker(mtx2);
//				job(source2);
//
//				if (source2 >= 1000000000)
//					source2 = 0;
//			}
//		}
//	});
//
//	//2번쓰레드
//	threadVec.emplace_back([&source, &source2]
//	{
//		while(1) {
//			{
//				lock_guard<recursive_mutex> locker(mtx);
//				job(source);
//
//				if (source >= 1000000000)
//					source = 0;
//			}
//
//			{
//				lock_guard<recursive_mutex> locker(mtx2);
//				job(source2);
//
//				if (source2 >= 1000000000)
//					source = 0;
//			}
//		}
//	});
//
//	for (int i = 0; i < threadVec.size(); i++) {
//		threadVec[i].join();
//	}
//	Clock::Stop();
//
//	cout << "source 1 : " << source << endl;
//	cout << "source 2 : " << source << endl;
//
//	return 0;
//}