// future_errc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <future>

using namespace std;

int main()
{
	promise<int> prom;
	try
	{
		prom.get_future();
		prom.get_future();	//	throw std::future_error with future_already_retrived
	}
	catch (const future_error& e)
	{
		if (e.code() == std::make_error_condition(future_errc::future_already_retrieved))
		{
			cerr << "[exception caught]: " << e.what() << '\n';
		}
		else
			cerr << "[unknown exception]\n";
	}
	catch (const std::exception&)
	{

	}

    return 0;
}

