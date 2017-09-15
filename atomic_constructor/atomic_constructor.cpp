// atomic_constructor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <iostream>
#include <thread>
#include <atomic>

std::atomic<bool> ready(false);
std::atomic_flag winner = ATOMIC_FLAG_INIT;

void count1m(int id)
{
	while (!ready)
	{
		std::this_thread::yield();
	}
	for (volatile int i = 0; i < 1000000; ++i)
	{
		
	}
	if (!winner.test_and_set())
	{
		std::cout << "thread #" << id << " won!\n";
	}
}


int main()
{
	std::vector<std::thread> vecThread;
	std::cout << "spawning 10 threads that count to 1 million...\n";
	for (int i = 0; i < 10; ++i)
	{
		vecThread.push_back(std::thread(count1m, i));
	}
	ready = true;
	for (auto& x : vecThread)
	{
		x.join();
	}

    return 0;
}

