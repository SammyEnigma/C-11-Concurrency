// promise.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <numeric>

void print_int(std::future<int>& fut)
{
	int x = fut.get();
	std::cout << "value " << x << '\n';
}

void accumulate(std::vector<int>::iterator first,
	std::vector<int>::iterator last,
	std::promise<int> accumulate_promise)
{
	auto sum = std::accumulate(first, last, 0);
	accumulate_promise.set_value(sum);		// Notify future
}

void do_work(std::promise<void> barrier)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	barrier.set_value();
}

void des_promise(std::future<int>& fut)
{
	try
	{
		auto x = fut.get();
		std::cout << "value is" << x << '\n';
	}
	catch (const std::future_error& e)
	{
		std::cerr << "[exception caught]: " << e.what() << '\n';
	}
	catch (const std::exception&)
	{
		std::cerr << "[unknown exception]\n";
	}
}

int main()
{
	/*			Example_1		*/
	std::promise<int> prom;				// create promise
	auto fut = prom.get_future();		//	engagement with future
	std::thread th1(print_int, std::ref(fut));		//	send future to new thread
	prom.set_value(10);					// fulfill promise
	th1.join();


	/*			Example_2		*/
	std::vector<int> numbers{ 1, 2, 3, 4, 5, 6 };
	std::promise<int> accumulate_promise;
	auto accumulate_future = accumulate_promise.get_future();
	std::thread work_thread(accumulate, numbers.begin(), numbers.end(), std::move(accumulate_promise));
	accumulate_future.wait();
	std::cout << "result = " << accumulate_future.get() << '\n';
	work_thread.join();


	/*			Example_3		*/
	std::promise<void> barrier;
	auto barrier_future = barrier.get_future();
	std::thread new_work_thread(do_work, std::move(barrier));
	barrier_future.wait();
	new_work_thread.join();


	/*			Example_4		*/
	std::future<int> f;
	{
		std::promise<int> p;
		f = p.get_future();
	}
	std::thread(des_promise, std::ref(f)).join();

    return 0;
}

