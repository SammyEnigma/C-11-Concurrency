// future_get.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <future>

using namespace std;

int get_int()
{
	cin.exceptions(ios::failbit);
	int x;
	cin >> x;
	return x;
}

int main()
{
	auto fut = async(get_int);
	cout << "Please, enter an integer value: ";
	try
	{
		int x = fut.get();
		cout << "value " << x << '\n';
	}
	catch (const std::exception& e)
	{
		cout << "[exception exception]: " << e.what() << '\n';
	}

    return 0;
}

