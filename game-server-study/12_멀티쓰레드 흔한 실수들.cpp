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
//1. �б�� ���� ��� ������� �ʱ�
//��� : ���� ���� ������ �����ϰų� ���� �ʱ� ������ �����ϴٰ� ���������� ���� ����ġ ���� ���� ���� �� �ִ�.
int var1;
mutex mtx1;

void ������_1() {
	//mtx.lock() ����
	printf("%d", var1);
}

void ������_2() {
	mtx1.lock();
	var1 += 1;
}


/****************************************/
//2. ��� ���� ����
//��� : 1�� �����忡���� 1->2->3������ ������� 
//       2�� �����忡���� 1->3->2 ������ ��� ������� �߻��� ���� �ִ�.
//�̷��� ��Ģ�� ��Ű�� �����Ƿ� �ִ��� ���� �����ϴ°� ����.
int a;
int b;
int c;
mutex mtx1;
mutex mtx2;
mutex mtx3;

//1�� �����忡���� 1 -> 2 -> 3 ������ ��ݼ����� �����Ͽ���.
void ������_1() {
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

//2�� �����忡���� 1 -> 3 -> 2 ������ ��ݼ����� �����ߴ�.
void ������_2() {
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
//3. �ʹ� ���� ��ݹ���
//��� : ��ݹ����� �ʹ� ���� ��� ���ļ��� ����� ���� �ִ�.
//       ��ݹ����� �ʹ� ���� ��� �ʹ� ����ϰ� ���ؽ�Ʈ ����ġ�� �߻��Ͽ� �������ϰ� ���ϰ� ���� �� �ִ�.

//�����ϰ� ������ ����
//�ʹ� ���� ���� ���
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

//�����ϰ� ���� ���
class B {
	int a;
	int a_count;
	int a_countList;
	mutex a_mtx;

	int b;
	mutex b_mtx;
};


/****************************************/
//4. ����̽� Ÿ���� ���� ���
//��� : printf�� ���� �Լ��� �α׸� ����Ҷ� �α׿� ������ ���� ��Ź����� ���������� �߻��� �� �ִ�.

void ������_1() {
	mtx1.lock();
	a += 1;
	b += 2;
	a == 1;

	printf("%d", a);
	mtx1.unlock();
}

/****************************************/
//5. ����� ���������� �߻��� �Ǽ�
//��� : �Ӱ迵���� ������ �߸���� �߸��� ������� ���� �� �ִ�. 

void ������_1() {
	listMtx.lock();
	Data* data = list.getData();
	listMtx.unlock();

	data->source = 500; 
	data->count = 1;
	//����Ʈ���� �����͸� �����ͼ� ���� ���� �ϰԵȴ�. �̶� �ٸ� �����嵵 ������ �����ϰ� �ǹ����Ƿ� �߸��� ���� ���� ������ �ִ�.
	//���⿡�� ����� �ɾ������.
}


/****************************************/
//6. ��ݵ� ���ؽ��� �Ӱ� ���������� ����
//��� : 

void ������_1() {
	Data* data = new Data();
	dataMtx.lock();
	delete data;

}

/****************************************/
//6. ��ݵ� ���ؽ��� �Ӱ� ���������� ����
//��� : 

void ������_1() {
	Data* data = new Data();
	dataMtx.lock();
	delete data;

}

/****************************************/
//7. �ϰ��� ��Ģ ����
//��� : list�Ӱ迵������ �����͸� �����ͼ� ������ �� listCount ���� �Ӱ迵������ �����༭ ��ĩ ����� ­�ٰ� ������ ���� �ִ�.
//������ ����Ʈ���� �����͸� ������ �� 

LinkedList list;
int listCount;

void ������_1() {
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

void ������_������() {
	int i = queue.pop();
	item = i;
}

void ����ϴ�_������() {
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