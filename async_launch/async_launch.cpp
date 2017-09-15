// async_launch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <future>
#include <thread>
#include <chrono>

void print_ten(char c, int ms)
{
	for (int i = 0; i < 10; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		std::cout << c;
	}
}


int main()
{
	std::cout << "with lauch::async:\n";
	auto foo = std::async(std::launch::async, print_ten, '*', 100);
	auto bar = std::async(std::launch::async, print_ten, '@', 200);
	// async "get" (wait for foo and bar to be ready):
	foo.get();
	bar.get();
	std::cout << "\n\n";

	std::cout << "with lauch::deferred:\n";
	foo = std::async(std::launch::deferred, print_ten, '*', 100);
	bar = std::async(std::launch::deferred, print_ten, '@', 200);
	// deferred "get" (perform the actual calls):
	foo.get();
	bar.get();
	std::cout << '\n';

    return 0;
}

