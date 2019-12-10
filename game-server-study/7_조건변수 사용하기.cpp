//#include <Windows.h>
//#include <thread>
//#include <iostream>
//#include <vector>
//#include <mutex>
//
//using namespace std;
//
//void workerThread(condition_variable* cv, mutex* mx, int* shared_resource, bool* conditionOk) {
//	while (1) {
//		unique_lock<mutex> locker(*mx);
//
//		cv->wait(locker, [&conditionOk] {
//			cout << conditionOk << " / " << *conditionOk << endl;
//			return *conditionOk; 
//		});
//		
//		(*shared_resource)++;
//		(*conditionOk) = false;
//
//		cout << this_thread::get_id() << " : 워커 쓰레드가 일어났습니다. 현재 공유자원 값 : " << *shared_resource << endl;
//	}
//}
//
//
//int main() {
//	mutex mx;
//	condition_variable cv;
//	int resource = 0;
//	bool condition1 = false;
//	bool condition2 = false;
//	bool condition3 = false;
//
//	thread t1(workerThread, &cv, &mx, &resource, &condition1);
//	thread t2(workerThread, &cv, &mx, &resource, &condition2);
//	thread t3(workerThread, &cv, &mx, &resource, &condition3);
//
//	Sleep(2000);
//	{
//		unique_lock<mutex> locker(mx);
//		condition1 = true;
//		condition2 = false;
//		condition3 = true;
//		cout << "일어나라 쓰레드여!" << endl;
//		cv.notify_all();
//	}
//
//
//	while (1);
//
//	t1.join();
//	t2.join();
//
//	return 0;
//}