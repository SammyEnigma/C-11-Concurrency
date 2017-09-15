// atomic_operator_T.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

std::atomic<int> foo = 0;
std::atomic<int> bar = 0;

void set_foo(int x)
{
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "set foo value\n";
	foo = x;
}

void copy_foo_to_bar()
{
	while (foo == 0)
	{
		std::this_thread::yield();
	}
	bar = static_cast<int>(foo);
}

void print_bar()
{
	while (bar == 0)
	{
		std::this_thread::yield();
	}
	std::cout << "bar: " << bar << '\n';
}

int main()
{
	std::thread th1(set_foo, 10);
	std::thread th2(copy_foo_to_bar);
	std::thread th3(print_bar);

	th1.join();
	th2.join();
	th3.join();

    return 0;
}

