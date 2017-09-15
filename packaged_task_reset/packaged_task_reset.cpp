// packaged_task_reset.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <future>
#include <thread>

using namespace std;


int main()
{
	auto lam = [](int x) { return x * 3; };
	packaged_task<int(int)> tsk(lam);
	auto fut = tsk.get_future();
	tsk(33);
	cout << "The triple of 33 is " << fut.get() << ".\n";

	//	re-use same task object
	tsk.reset();
	fut = tsk.get_future();
	std::thread(std::move(tsk), 99).detach();
	cout << "The triple of 99 is " << fut.get() << ".\n";

    return 0;
}

