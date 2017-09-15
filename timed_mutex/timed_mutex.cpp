// timed_mutex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <ctime>

std::timed_mutex tmt;

void fireworks()
{
	while (!tmt.try_lock_for(std::chrono::milliseconds(200)))
	{
		std::cout << "-";
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "*\n";
	tmt.unlock();
}

std::chrono::time_point<std::chrono::system_clock> midnight()
{
	using std::chrono::system_clock;
	std::time_t tt = system_clock::to_time_t(system_clock::now());
	std::tm* ptm = std::localtime(&tt);
	++ptm->tm_mday;
	ptm->tm_hour = 0;
	ptm->tm_min = 0;
	ptm->tm_sec = 0;
	return system_clock::from_time_t(mktime(ptm));
}

void carriage()
{
	if (tmt.try_lock_until(midnight()))
	{
		std::cout << "ride back home on carrage\n";
		tmt.unlock();
	}
	else
		std::cout << "carriage reverts to pumpkin\n";
}

void ball()
{
	tmt.lock();
	std::cout << "at the ball..\n";
	tmt.unlock();
}

int main()
{
	/*std::thread task[10];

	for (int i = 0; i < 10; ++i)
	{
		task[i] = std::thread(fireworks);
	}

	for (auto& x : task)
	{
		x.join();
	}*/

	std::thread th1(carriage);
	std::thread th2(ball);

	th1.join();
	th2.join();

    return 0;
}

