// condition_variable.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;

void wait_task()
{
	std::unique_lock<std::mutex> lck(mtx);
	cv.wait(lck);
	std::cout << "thread " << std::this_thread::get_id() << " wake up.\n";
}

void worker()
{
	std::unique_lock<std::mutex> lck(mtx);
	std::this_thread::sleep_for(std::chrono::seconds(5));
	cv.notify_all();
	std::cout << "worker notify\n";
}


int main()
{
	std::thread task[10];
	for (int i = 0; i < 10; ++i)
	{
		task[i] = std::thread(wait_task);
	}

	std::thread work(worker);
	work.join();

	for (auto& x : task)
	{
		x.join();
	}

    return 0;
}

