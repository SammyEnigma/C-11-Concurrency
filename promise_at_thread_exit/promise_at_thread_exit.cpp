// promise_at_thread_exit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <future>
#include <chrono>

#define cout (std::cout)

int main()
{
	std::promise<int> p;
	auto fut = p.get_future();
	std::thread([&p] { 
		std::this_thread::sleep_for(std::chrono::seconds(10));
		p.set_value_at_thread_exit(10);
	}).detach();

	cout << "Waiting..." << std::endl;
	fut.wait();
	cout << "Done!\nResult is: " << fut.get() << std::endl;

    return 0;
}

