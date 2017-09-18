// atomic_flag.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <sstream>

std::atomic_flag lock_stream = ATOMIC_FLAG_INIT;
std::stringstream stream;

void append_number(int i)
{
	while (lock_stream.test_and_set())
	{
	}

	stream << "thread #" << i << '\n';
	lock_stream.clear();
}


int main()
{
	std::vector<std::thread> vecThread;
	for (int i = 0; i < 10; ++i)
	{
		vecThread.push_back(std::thread(append_number, i));
	}

	for (auto& x : vecThread)
	{
		x.join();
	}

	std::cout << stream.str();

    return 0;
}

