#include "ObjectPool.h"
#include "ConcurrentAlloc.h"

void Alloc1()
{
	for (size_t i = 0; i < 5; ++i)
	{
		void* ptr = ConcurrentAlloc(6);
	}
}

void Alloc2()
{
	for (size_t i = 0; i < 5; ++i)
	{
		void* ptr = ConcurrentAlloc(7);
	}
}

void TLSTest()
{
	std::thread t1(Alloc1);
	t1.join();

	std::thread t2(Alloc2);
	t2.join();
}

// ���������ͷ�7�θ�8byte�ڲ��ȵļ��������������ͷ�
// ͨ�����Թ۲���ʾ��������Ĺ��̺��ͷŹ��̣�
// �����������Ҳ�ָ���з�,�ͷŹ������ֳ�ҳ�ĺϲ���
void TestConcurrentAlloc1()
{
	void* p1 = ConcurrentAlloc(6);
	void* p2 = ConcurrentAlloc(8);
	void* p3 = ConcurrentAlloc(1);
	void* p4 = ConcurrentAlloc(7);
	void* p5 = ConcurrentAlloc(8);
	void* p6 = ConcurrentAlloc(8);
	void* p7 = ConcurrentAlloc(8);


	cout << p1 << endl;
	cout << p2 << endl;
	cout << p3 << endl;
	cout << p4 << endl;
	cout << p5 << endl;
	cout << p6 << endl;
	cout << p7 << endl;


	ConcurrentFree(p1);
	ConcurrentFree(p2);
	ConcurrentFree(p3);
	ConcurrentFree(p4);
	ConcurrentFree(p5);
	ConcurrentFree(p6);
	ConcurrentFree(p7);
}

// ���������ͷ�1024��8byte�ڲ��ȵļ�����������룬��1024������ѵ�һ��ҳ�����ˡ�
// ��ϵ㣬ͨ�����Թ۲���ʾ�����1025�����룬��page cacheȥ��һ����span����֤ǰ1024��������з��߼�
void TestConcurrentAlloc2()
{
	for (size_t i = 0; i < 1024; ++i)
	{
		void* p1 = ConcurrentAlloc(6);
		cout << p1 << endl;
	}

	void* p2 = ConcurrentAlloc(8);
	cout << p2 << endl;
}

void TestAddressShift()
{
	PAGE_ID id1 = 2000;
	PAGE_ID id2 = 2001;
	char* p1 = (char*)(id1 << PAGE_SHIFT);
	char* p2 = (char*)(id2 << PAGE_SHIFT);
	while(p1 < p2)
	{
		cout << (void*)p1 <<":"<< ((PAGE_ID)p1 >> PAGE_SHIFT) << endl;
		p1 += 8;
	}
}

// ���������̷ֱ߳���һ�������ͷ�10�Σ���֤�����Ķ��̳߳����������ͷ��Ƿ�������
void MultiThreadAlloc1()
{
	std::vector<void*> v;
	for (size_t i = 0; i < 10; ++i)
	{
		void* ptr = ConcurrentAlloc(6);
		cout <<std::this_thread::get_id() <<ptr << endl;
		v.push_back(ptr);
	}

	for (auto e : v)
	{
		ConcurrentFree(e);
	}
}

void MultiThreadAlloc2()
{
	std::vector<void*> v;
	for (size_t i = 0; i < 10; ++i)
	{
		void* ptr = ConcurrentAlloc(16);
		cout << std::this_thread::get_id() << ptr << endl;
		v.push_back(ptr);
	}

	for (auto e : v)
	{
		ConcurrentFree(e);
	}
}

void TestMultiThread()
{
	std::thread t1(MultiThreadAlloc1);
	std::thread t2(MultiThreadAlloc2);

	t1.join();
	t2.join();
}

// �����ͷ�һ��257KB�ڴ棬��֤����ڴ������ͷţ����Թ۲���page cache�зֺͺϲ����ҳ���߼�
// �����ͷ�һ��129*8KB�ڴ棬��֤������ڴ������ͷţ����Թ۲���������ͷ��ڴ��߼�
void BigAlloc()
{
	void* p1 = ConcurrentAlloc(257 * 1024);
	ConcurrentFree(p1);

	void* p2 = ConcurrentAlloc(129 * 8 * 1024);
	ConcurrentFree(p2);
}

//int main()
//{
//	//TestObjectPool();
//	//TLSTest();
//
//	//TestConcurrentAlloc1();
//	//TestAddressShift();
//
//	//TestMultiThread();
//
//	//BigAlloc();
//
//	return 0;
//}