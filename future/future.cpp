// future.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <future>
#include <chrono>

using namespace std;

bool is_prime(int x)
{
	for (int i = 2; i < (int)sqrt(x); ++i)
	{
		if (x%i == 0)
		{
			return false;
		}
	}
	return true;
}


int main()
{
	auto fut = async(is_prime, 444444443);
	cout << "checking, please wait";
	std::chrono::microseconds span(100);
	while (fut.wait_for(span) == future_status::timeout)
	{
		cout << '.' << flush;
	}
	auto shfut = fut.share();
	cout << "\nthis value is" << (shfut.get() ? " prime" : " not prime") << '\n';
	cout << "\nthis value is" << (shfut.get() ? " prime" : " not prime");
	cout << "\n future is " << (fut.valid() ? " valid" : " invalid");
	cout << "\n shared_future is " << (shfut.valid() ? " valid" : " invalid");
	cout << '\n';

    return 0;
}

