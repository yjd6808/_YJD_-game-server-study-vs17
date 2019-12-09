//#include "Clock.h"
//#include <iostream>
//#include <thread>
//#include <vector>
//#include <mutex>
//
//
//using namespace std;
//recursive_mutex mx;
//
//bool isPrime(int num) {
//	if (num == 1 || num == 0)
//		return false;
//
//	if (num == 2 || num == 3) 
//		return true;
//
//	for (int i = 2; i < num - 1; i++) {
//		if ((num % i) == 0)
//			return false;
//	}
//
//	return true;
//}
//
//void countPrimesWithSingleThread(int maxNumber) {
//	Clock::Start();
//	vector<int> primes;
//	for (int i = 0; i < maxNumber; i++) {
//		if (isPrime(i))
//			primes.push_back(i);
//	}
//	Clock::Stop();
//	cout << "single thread 0 to " << maxNumber << " prime count : " << primes.size() << endl;
//}
//
//
//void pushPrimeRange(vector<int>* primeVec, int min, int max) {
//	
//	for (int i = min; i < max; i++) {
//		if (isPrime(i)) {
//			lock_guard<recursive_mutex> locker(mx);
//			primeVec->push_back(i);
//		}
//	}
//}
//
//void countPrimesWithThreads(int maxNumber, int threadCount) {
//	vector<thread> threadVec;
//	vector<int> primeVec;
//	
//	Clock::Start();
//	for (int i = 0; i < threadCount; i++) {
//		int chunck = maxNumber / threadCount;
//		int min = chunck * i;
//		int max = chunck * (i + 1);
//		threadVec.push_back(thread(pushPrimeRange, &primeVec, min, max));
//	}
//
//	for (int i = 0; i < threadCount; i++)
//		threadVec[i].join();
//
//
//	Clock::Stop();
//	cout << "multy thread 0 to " << maxNumber << " prime count : " << primeVec.size() << endl;
//}
//
//
//int main() {
//
//	countPrimesWithSingleThread(300000);
//	countPrimesWithThreads(300000, 6);
//}