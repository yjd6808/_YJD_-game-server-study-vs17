//#include <Windows.h>
//#include <thread>
//#include <iostream>
//#include <mutex>
//
//using namespace std;
//
///****************************************/
////���� ���� ���� ������ notify ���ϰ� ����
////��� : �ƹ��͵� ��¾ȵ�
//condition_variable cv;
//mutex mx;
//
//void �Լ�1() {
//	thread t1([]
//	{
//		unique_lock <mutex> locker(mx);
//		cv.wait(locker);
//		cout << "�ȳ��ϼ���" << endl;
//	});
//
//	t1.join();
//}
//
///****************************************/
////���� ���� ���� ���� notify�� �����
////��� : �ȳ��ϼ���
//condition_variable cv2;
//mutex mx2;
//
//void �Լ�2() {
//	thread t1([]
//	{
//		unique_lock <mutex> locker(mx2);
//		cv2.wait(locker);
//		cout << "�ȳ��ϼ���" << endl;
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
//	�Լ�1();
//	return 0;
//}