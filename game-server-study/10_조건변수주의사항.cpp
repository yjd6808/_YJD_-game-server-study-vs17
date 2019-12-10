//#include <Windows.h>
//#include <thread>
//#include <iostream>
//#include <mutex>
//
//using namespace std;
//
///****************************************/
////조건 없는 조건 변수를 notify 안하고 보기
////결과 : 아무것도 출력안됨
//condition_variable cv;
//mutex mx;
//
//void 함수1() {
//	thread t1([]
//	{
//		unique_lock <mutex> locker(mx);
//		cv.wait(locker);
//		cout << "안녕하세요" << endl;
//	});
//
//	t1.join();
//}
//
///****************************************/
////조건 없는 조건 변수 notify로 깨우기
////결과 : 안녕하세요
//condition_variable cv2;
//mutex mx2;
//
//void 함수2() {
//	thread t1([]
//	{
//		unique_lock <mutex> locker(mx2);
//		cv2.wait(locker);
//		cout << "안녕하세요" << endl;
//	});
//
//	thread t2([]
//	{
//		Sleep(1000);
//		cv2.notify_one();
//	});
//
//	t1.join();
//	t2.join();
//}
//
//int main() {
//	
//	함수1();
//	return 0;
//}