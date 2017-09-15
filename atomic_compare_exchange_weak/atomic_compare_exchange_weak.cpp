// atomic_compare_exchange_weak.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <thread>
#include <atomic>

struct Node
{
	int value;
	Node* next;
};
std::atomic<Node*> list_head(nullptr);

//	append an element to the list:
void apend(int val)
{
	Node* oldHead = list_head;
	Node* newNode = new Node{ val, oldHead };

	/*	what follows is equivalent to: list_head = newNode, but in a thread-safe way:
	*	当前线程在比较的时候，有可能别的线程修改过list_head了，造成oldHead和list_node不同，newNode的next需要更新；
	*/
	while (!list_head.compare_exchange_weak(oldHead, newNode))
	{
		newNode->next = oldHead;
	}
}

int main()
{
	//	spawn 10 threads to fill the linked list:
	std::vector<std::thread> vecThreads;
	for (int i = 0; i < 10; ++i)
	{
		vecThreads.push_back(std::thread(apend, i));
	}
	for (auto& x : vecThreads)
	{
		x.join();
	}

	// print contents:
	for (Node* it = list_head; it != nullptr; it = it->next)
	{
		std::cout << " " << it->value;
	}
	std::cout << '\n';

	//	cleanup:
	Node* it = nullptr;
	while (it = list_head)
	{
		list_head = it->next;
		delete it;
		it = nullptr;
	}

    return 0;
}

