//#include <Windows.h>
//#include <thread>
//#include <iostream>
//#include <mutex>
//#include <atomic>
//
//using namespace std;
//
//
//atomic<int> a;
//
//int main() {
//	thread t1([] 
//	{
//		for (int i = 0; i < 50000000; i++)
//			a++;;
//	});
//
//	thread t2([]
//	{
//		for (int i = 0; i < 50000000; i++)
//			a--;
//	});
//
//	t1.join();
//	t2.join();
//
//	cout << "°á°ú : " << a << endl;
//
//	return 0;
//}