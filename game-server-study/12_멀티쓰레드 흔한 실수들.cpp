#include <Windows.h>
#include <thread>
#include <iostream>
#include <mutex>
#include <vector>
#include <atomic>
#include <queue>
#include <crtdbg.h>

using namespace std;


mutex mtx1;
mutex mtx2;
mutex mtx3;

/****************************************/
//1. 읽기와 쓰기 모두 잠금하지 않기
//결과 : 읽을 때는 변수에 대입하거나 하지 않기 때문에 안전하다고 생각하지만 가끔 예상치 못한 값이 들어올 수 있다.
int var1;
mutex mtx1;

void 쓰레드_1() {
	//mtx.lock() 누락
	printf("%d", var1);
}

void 쓰레드_2() {
	mtx1.lock();
	var1 += 1;
}


/****************************************/
//2. 잠금 순서 어기기
//결과 : 1번 쓰레드에서는 1->2->3순으로 잠궜지만 
//       2번 쓰레드에서는 1->3->2 순으로 잠궈 데드락이 발생할 수도 있다.
//이러한 규칙은 지키기 어려우므로 최대한 적게 유지하는게 좋다.
int a;
int b;
int c;
mutex mtx1;
mutex mtx2;
mutex mtx3;

//1번 쓰레드에서는 1 -> 2 -> 3 순으로 잠금순서를 유지하였다.
void 쓰레드_1() {
	mtx1.lock();
	a += 1;
	mtx1.unlock();
	mtx2.lock();
	b += 1;
	mtx2.unlock();
	mtx3.lock();
	c += 1;
	mtx3.unlock();
}

//2번 쓰레드에서는 1 -> 3 -> 2 순으로 잠금순서를 유지했다.
void 쓰레드_2() {
	mtx1.lock();
	a -= 1;
	mtx1.unlock();
	mtx3.lock();
	b -= 1;
	mtx3.unlock();
	mtx2.lock();
	c -= 1;
	mtx2.unlock();
}

/****************************************/
//3. 너무 좁은 잠금범위
//결과 : 잠금범위가 너무 넓을 경우 병렬성이 퇴색될 수도 있다.
//       잠금범위가 너무 좁을 경우 너무 빈번하게 컨텍스트 스위치가 발생하여 성능저하가 심하게 생길 수 있다.

//적당하게 나누어 쓰자
//너무 좁게 잡은 경우
class A {
	int a;
	mutex a_mtx;
	int a_count;
	mutex acount_mtx;
	int a_countList;
	mutex acountList_mtx;

	int b;
	mutex b_mtx;
};

//적당하게 잡은 경우
class B {
	int a;
	int a_count;
	int a_countList;
	mutex a_mtx;

	int b;
	mutex b_mtx;
};


/****************************************/
//4. 디바이스 타임이 섞인 잠금
//결과 : printf와 같은 함수로 로그를 출력할때 로그와 연산을 같이 잠궈버려서 병목현상이 발생할 수 있다.

void 쓰레드_1() {
	mtx1.lock();
	a += 1;
	b += 2;
	a == 1;

	printf("%d", a);
	mtx1.unlock();
}

/****************************************/
//5. 잠금의 전염성으로 발생한 실수
//결과 : 임계영역의 범위를 잘못잡아 잘못된 결과값을 얻을 수 있다. 

void 쓰레드_1() {
	listMtx.lock();
	Data* data = list.getData();
	listMtx.unlock();

	data->source = 500; 
	data->count = 1;
	//리스트에서 데이터를 가져와서 값을 변경 하게된다. 이때 다른 쓰레드도 접근이 가능하게 되버리므로 잘못된 값을 얻을 여지가 있다.
	//여기에서 잠금을 걸어줘야함.
}


/****************************************/
//6. 잠금된 뮤텍스나 임계 영역에서의 삭제
//결과 : 

void 쓰레드_1() {
	Data* data = new Data();
	dataMtx.lock();
	delete data;

}

/****************************************/
//6. 잠금된 뮤텍스나 임계 영역에서의 삭제
//결과 : 

void 쓰레드_1() {
	Data* data = new Data();
	dataMtx.lock();
	delete data;

}

/****************************************/
//7. 일관성 규칙 깨기
//결과 : list임계영역에서 데이터를 가져와서 삭제한 후 listCount 값을 임계영역에서 내려줘서 자칫 제대로 짯다고 생각할 수도 있다.
//하지만 리스트에서 데이터를 삭제한 후 

LinkedList list;
int listCount;

void 쓰레드_1() {
	listMtx.lock();
	Data* data = list.getData();
	list.remove(data);
	listMtx.unlock();

	listCountMtx.lock();
	listCount--;
	listCountMtx.unlock();
}

ParrelQueue<int> queue;
atomic<int> item;

void 꺼내는_쓰레드() {
	int i = queue.pop();
	item = i;
}

void 사용하는_쓰레드() {
	int i = item.exchange(0);
	if (i != 0) {
		...
	}
}



mutex a_mtx;
vector<thread> consumerVec;
queue<int*> productList;

int main() {


	return 0;
}