// lock_guard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <stdexcept>

std::mutex mt;

void print_even(int x)
{
	if (x % 2 == 0)
	{
		std::cout << x << " is even\n";
	}
	else
		throw (std::logic_error("not even"));
}

void print_thread_id(int id)
{
	try
	{
		std::lock_guard<std::mutex> lck(mt);
		print_even(id);
	}
	catch (const std::logic_error& exc)
	{
		std::cout << "[exception caught]" << exc.what() << '\n';
	}
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

