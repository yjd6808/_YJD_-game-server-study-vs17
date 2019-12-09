//#include "Clock.h"
//#include <iostream>
//#include <thread>
//#include <vector>
//#include <mutex>
//#include <Windows.h>
//
//
//using namespace std;
//
//
//
//class CriticalSection {
//	CRITICAL_SECTION m_critSec;
//public:
//	CriticalSection() {
//		InitializeCriticalSectionEx(&m_critSec, 0, 0);
//	}
//	~CriticalSection() {
//		DeleteCriticalSection(&m_critSec);
//	}
//
//	void Lock() {
//		EnterCriticalSection(&m_critSec);
//	}
//	void Unlock() {
//		LeaveCriticalSection(&m_critSec);
//	}
//};
//
//class CriticalSectionLock {
//	CriticalSection* m_pCritSec;
//public:
//	CriticalSectionLock(CriticalSection& critSec) {
//		m_pCritSec = &critSec;
//		m_pCritSec->Lock();
//	}
//	~CriticalSectionLock() {
//		m_pCritSec->Unlock();
//	}
//};
//
//int a = 0;
//int b = 0;
//CriticalSection mx;
//CriticalSection mx2;
//
//int main() {
//	
//	사용시 주석 해제 후 사용할 것
//	thread t1([]
//	{
//		for (int i = 0; i < 10000; i++) {
//			
//			CriticalSectionLock lock(mx);
//			cout << "t1 mx 잠금" << endl;
//			a++;
//			CriticalSectionLock lock2(mx2);
//			cout << "t1 mx2 잠금" << endl;
//			b++;
//
//			cout << "t1 mx 해제" << endl;
//			cout << "t1 mx2 해제" << endl;
//		}
//	});
//
//	thread t2([]
//	{
//		for (int i = 0; i < 10000; i++) {
//
//			CriticalSectionLock lock(mx2);
//			cout << "t2 mx2 잠금" << endl;
//			a++;
//			CriticalSectionLock lock2(mx);
//			cout << "t2 mx 잠금" << endl;
//			b++;
//
//			cout << "t2 mx 해제" << endl;
//			cout << "t2 mx2 해제" << endl;
//		}
//	});
//
//	cout << "쓰레드 1번 ID : " << t1.get_id() << endl;
//	cout << "쓰레드 2번 ID : " << t2.get_id() << endl;
//
//	t1.join();
//	t2.join();
//}