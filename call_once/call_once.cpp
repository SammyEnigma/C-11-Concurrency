// call_once.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

std::once_flag winner_flag;
int winner;

void set_winner(int x)
{
	winner = x;
}

void wait_1000ms(int id)
{
	for (int i = 0; i < 1000; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	std::call_once(winner_flag, set_winner, id);
}


int main()
{
	std::thread task[10];
	for (int i = 0; i < 10; ++i)
	{
		task[i] = std::thread(wait_1000ms, i + 1);
	}

	for (auto& x : task)
	{
		x.join();
	}

	std::cout << "winner thread: " << winner << '\n';

    return 0;
}

