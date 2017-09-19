// atomic_store.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> foo(0);

void set_foo(int x)
{
	foo.store(x, std::memory_order_relaxed);	//	set value atomically
}

void print_foo()
{
	int x;
	do 
	{
		x = foo.load(std::memory_order_relaxed);	// get value atomically
	} while (x == 0);
	std::cout << "foo: " << x << '\n';
}


int main()
{
	std::thread th1(print_foo);
	std::thread th2(set_foo, 10);
	th2.join();
	th1.join();

    return 0;
}

