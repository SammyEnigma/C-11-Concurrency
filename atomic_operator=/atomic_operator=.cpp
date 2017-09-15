// atomic_operator=.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<int> foo = 0;

void set_foo(int x)
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout << "start set x value\n";
	std::cout << std::boolalpha;
	std::cout << foo.is_lock_free() << '\n';
	std::cout << std::noboolalpha;
	foo = x;
}

void print_foo()
{
	while (foo == 0)
	{
		std::this_thread::yield();
	}
	std::cout << "foo: " << foo << '\n';
}


int main()
{
	std::thread th1(print_foo);
	std::thread th2(set_foo, 10);
	th2.join();
	th1.join();

    return 0;
}

