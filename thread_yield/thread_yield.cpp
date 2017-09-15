// thread_yield.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <atomic>

std::atomic<bool> ready(false);

void count1m(int id)
{
	while (!ready)
	{
		std::this_thread::yield();
	}

	for (volatile int i = 0; i < 1000000; ++i)
	{
		
	}
	std::cout << id;
}


int main()
{
	std::thread task[10];
	for (int i = 0; i < 10; ++i)
	{
		task[i] = std::thread(count1m, i);
	}
	ready = true;
	for (auto& th : task)
	{
		th.join();
	}
	std::cout << '\n';

    return 0;
}

