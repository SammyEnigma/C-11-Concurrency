// cv_wair_for.cpp : Defines the entry point for the console application.
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
	/*std::cout << "Please, enter an integer(I'll be printing dots):\n";
	int x;
	std::thread th([&] {
		std::cin >> x;
		cv.notify_one();
	});

	std::unique_lock<std::mutex> lck(mtx);
	while (cv.wait_for(lck, std::chrono::seconds(10)) == std::cv_status::timeout)
		std::cout << ".";

	std::cout << "\nYou entered: " << x << '\n';
	th.join();*/

	int x = 0;
	std::thread work([&x] {
		std::cin >> x;
		cv.notify_one();
	});
	std::unique_lock<std::mutex> lck(mtx);
	while (!cv.wait_for(lck, std::chrono::seconds(10), [x] { return (x != 0); }))
		std::cout << ".";

	std::cout << "\nYou entered: " << x << '\n';
	work.join();


    return 0;
}

