// lock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex foo;
std::mutex bar;

void task1()
{
	// foo.lock(); bar.lock(); // replaced by:
	std::lock(foo, bar);
	std::cout << "thread # 1\n";
	foo.unlock();
	bar.unlock();
}

void task2()
{
	std::lock(bar, foo);
	std::cout << "thread # 2\n";
	foo.unlock();
	bar.unlock();
}


int main()
{
	std::thread th1(task1);
	std::thread th2(task2);

	th1.join();
	th2.join();

    return 0;
}

