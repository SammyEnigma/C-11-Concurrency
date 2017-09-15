// packaged_task.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <future>
#include <thread>
#include <chrono>

using namespace std;

int countdown(int from, int to)
{
	for (int i = from; i != to; --i)
	{
		cout << i << '\n';
		this_thread::sleep_for(chrono::seconds(1));
	}
	cout << "Lift off!\n";
	return (from - to);
}

int main()
{
	packaged_task<decltype(countdown)> tsk(countdown);
	auto fut = tsk.get_future();
	std::thread th(std::move(tsk), 10, 0);

	cout << "The countdown lasted for " << fut.get() << " seconds.\n";
	th.join();

    return 0;
}

