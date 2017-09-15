// mutex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

void print_thread_id(int id)
{
	mtx.lock();
	std::cout << "thread #" << id << '\n';
	mtx.unlock();
}

int main()
{
	std::thread task[10];
	for (int i = 0; i < 10; ++i)
	{
		task[i] = std::thread(print_thread_id, i + 1);
	}

	for (auto& x : task)
	{
		x.join();
	}

    return 0;
}

