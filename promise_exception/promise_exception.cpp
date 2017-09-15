// promise_exception.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <future>

void get_int(std::promise<int>& prom)
{
	std::cout << "Please, enter an integer value: ";
	std::cin.exceptions(std::ios::failbit);		// throw on failbit
	int x;
	try
	{
		std::cin >> x;							//	sets failbit if input is not int
		prom.set_value(x);
	}
	catch (const std::exception&)
	{
		prom.set_exception(std::current_exception());
	}
}

void print_int(std::future<int>& fut)
{
	try
	{
		auto x = fut.get();
		std::cout << "value " << x << '\n';
	}
	catch (const std::exception& e)
	{
		std::cout << "[exception caught: " << e.what() << "]\n";
	}
}


int main()
{
	std::promise<int> prom;
	auto fut = prom.get_future();
	std::thread th1(get_int, std::ref(prom));
	std::thread th2(print_int, std::ref(fut));

	th1.join();
	th2.join();

    return 0;
}

