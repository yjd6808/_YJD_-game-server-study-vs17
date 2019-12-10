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
//		cout << "��� ����! �۾����ҷ�!" << endl;
//		return;
//	}
//	else if (ret == WAIT_ABANDONED) {
//		cout << "�̺�Ʈ ������!" << endl;
//		ResetEvent(handle);
//	}
//	else if (ret == WAIT_TIMEOUT) {
//		cout << "�۾� Ÿ�� �ƿ�!" << endl;
//	}
//	else if (ret == WAIT_OBJECT_0) {
//		ResetEvent(handle);
//		cout << this_thread::get_id() << " : ��! �۾� ����!" << endl;
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
//					//�۾� ����...
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
//			cout << "���ض� ��������!" << endl;
//			SetEvent(event1);
//		}
//	});
//
//	
//	th2.join();
//
//	return 0;
//}