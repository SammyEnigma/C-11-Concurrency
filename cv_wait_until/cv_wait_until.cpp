// cv_wait_until.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;


int main()
{
	/*int x = 0;
	std::thread th([&x] {
		std::cin >> x;
		cv.notify_one();
	});

	std::unique_lock<std::mutex> lck(mtx);
	while (cv.wait_until(lck, std::chrono::system_clock::now() + std::chrono::seconds(10)) == std::cv_status::timeout)
	{
		std::cout << ".";
	}
	std::cout << "\nvalue is " << x << '\n';

	th.join();*/

	int x = 0;
	std::thread th([&x] {
		std::cin >> x;
		cv.notify_one();
	});

	std::unique_lock<std::mutex> lck(mtx);
	while (!cv.wait_until(lck, std::chrono::system_clock::now() + std::chrono::seconds(10), [x] {return (x != 0); }))
	{
		std::cout << ".";
	}
	std::cout << "\nvalue is " << x << '\n';

	th.join();

    return 0;
}

