// thread_2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <chrono>

void thread_task(int n)
{
	std::this_thread::sleep_for(std::chrono::seconds(n));
	std::cout << "hello, thread " << std::this_thread::get_id() << " paused " << n << " seconds\n";
}

int main()
{
	std::cout << "Ӳ��֧�ֵĲ�����: " << std::thread::hardware_concurrency() << "\n";

	std::thread task[5];
	for (int i = 0; i != 5; ++i)
	{
		task[i] = std::thread(thread_task, i + 1);
	}

	std::cout << "Done spawning threads! Now wait for them to join\n";
	for (auto& t : task)
	{
		t.join();
	}
	std::cout << "All threads joined.\n";

    return 0;
}

