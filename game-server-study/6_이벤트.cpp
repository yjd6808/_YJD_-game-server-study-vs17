//#include <Windows.h>
//#include <thread>
//#include <iostream>
//#include <vector>
//
//using namespace std;
//
//void doJobIfEventIsSignaled(HANDLE handle, function<void()> func) {
//	DWORD ret = WaitForSingleObject(handle, INFINITE);
//
//	if (ret == WAIT_FAILED) {
//		cout << "대기 실패! 작업안할래!" << endl;
//		return;
//	}
//	else if (ret == WAIT_ABANDONED) {
//		cout << "이벤트 버려짐!" << endl;
//		ResetEvent(handle);
//	}
//	else if (ret == WAIT_TIMEOUT) {
//		cout << "작업 타임 아웃!" << endl;
//	}
//	else if (ret == WAIT_OBJECT_0) {
//		ResetEvent(handle);
//		cout << this_thread::get_id() << " : 옙! 작업 시작!" << endl;
//		func();
//	}
//}
//
//int main() {
//	HANDLE event1 = CreateEvent(nullptr, true, false, nullptr);
//	vector<thread> threadVec;
//	for (int i = 0; i < 3; i++ ) {
//		threadVec.emplace_back(([&event1]
//		{
//			while (true) {
//				doJobIfEventIsSignaled(event1, [] {
//					//작업 수행...
//				});
//			}
//		}));
//	}
//	
//
//	thread th2([&event1]
//	{
//		while (true) {
//			Sleep(2000);
//			cout << "일해라 쓰레드들아!" << endl;
//			SetEvent(event1);
//		}
//	});
//
//	
//	th2.join();
//
//	return 0;
//}