//#include <Windows.h>
//#include <thread>
//#include <iostream>
//#include <vector>
//#include <mutex>
//
//using namespace std;
//
//condition_variable cv;
//mutex mx;
//
//int main() {
//	thread t1([] 
//	{
//		unique_lock <mutex> locker(mx);
//		cv.wait(locker);
//	});
//
//	t1.join();
//
//	return 0;
//}