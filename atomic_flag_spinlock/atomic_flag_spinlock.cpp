// atomic_flag_spinlock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

std::atomic_flag lock = ATOMIC_FLAG_INIT;


/*		结合std::atomic_flag::test_and_set()和std::atomic_flag_clear()，std::atomic_flag对象可以当做一个简单的自旋锁使用			*/
void f(int i)
{
	for (int cnt = 0; cnt < 100; ++cnt)
	{
		while (lock.test_and_set(std::memory_order_acquire))
		{
			std::this_thread::yield();
		}
		std::cout << "Output from thread " << i << '\n';
		lock.clear(std::memory_order_release);
	}
}


int main()
{
	std::vector<std::thread> vecThread;
	for (int i = 0; i < 10; ++i)
	{
		vecThread.push_back(std::thread(f, i));
	}

	for (auto& x : vecThread)
	{
		x.join();
	}

    return 0;
}

