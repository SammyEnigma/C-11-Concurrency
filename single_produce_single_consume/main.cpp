/************************************************************************/
/*  
	单生产者 - 单消费者模型中只有一个生产者和一个消费者，生产者不停地往产品库中放入产品，
	消费者则从产品库中取走产品，产品库容积有限制，只能容纳一定数目的产品，如果生产者生产产品的速度过快，
	则需要等待消费者取走产品之后，产品库不为空才能继续往产品库中放置新的产品，
	相反，如果消费者取走产品的速度过快，则可能面临产品库中没有产品可使用的情况，
	此时需要等待生产者放入一个产品后，消费者才能继续工作。C++11实现单生产者单消费者模型的代码如下：                                                                   
*/
/************************************************************************/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

static const int ITEM_REPOSITORY_SIZE = 10;		//	item buffer size
static const int ITEMS_TO_PRODUCE = 1000;		//	number of produce
											
/*	生产者任务	*/
static void ProduceTask();
/*	消费者任务	*/
static void ConsumeTask();

int main(int argc, char* argv[])
{
	std::thread producer(ProduceTask);
	std::thread consumer(ConsumeTask);

	producer.join();
	consumer.join();

	return 0;
}

typedef struct _tagItemRepository
{
	int buff[ITEM_REPOSITORY_SIZE];				//	产品缓冲区，配合read_pos和write_pos模型环形队列
	size_t read_pos;							//	消费者读取产品位置
	size_t write_pos;							//	生产者写入产品位置
	std::mutex mtx;								//	互斥量，保护产品缓冲区
	std::condition_variable cv_not_full;		//	条件变量，指示产品缓冲区不为满
	std::condition_variable cv_not_empty;		//	条件变量，指示产品缓冲区不为空

	_tagItemRepository()
	{
		memset(buff, 0, ITEM_REPOSITORY_SIZE);
		read_pos = 0;
		write_pos = 0;
	}
}ItemRepository, *PItemRepository;

ItemRepository gItemRepository;		//	全局变量，生产者和消费者操作该变量

void ProduceItem(PItemRepository ir, int item)
{
	std::unique_lock<std::mutex> lck(ir->mtx);
	if ((ir->write_pos + 1) % ITEM_REPOSITORY_SIZE == ir->read_pos)
	{
		std::cout << "Producer is waiting for an empty slot...\n";
		ir->cv_not_full.wait(lck);	//	等待不为满通知
	}

	ir->buff[ir->write_pos++] = item;	//	写入产品
	if (ir->write_pos == ITEM_REPOSITORY_SIZE)	//	写入位置若是队列最后则重新设置为初始位置
	{
		ir->write_pos = 0;
	}

	ir->cv_not_empty.notify_one();	//	通知消费者产品库不为空
	lck.unlock();
}

void ProduceTask()
{
	for (int i = 1; i <= ITEMS_TO_PRODUCE; ++i)
	{
		std::cout << "Produce the " << i << "^th item..." << std::endl;
		ProduceItem(&gItemRepository, i);
	}
}

int ConsumeItem(PItemRepository ir)
{
	std::unique_lock<std::mutex> lck(ir->mtx);
	if (ir->read_pos == ir->write_pos)
	{
		std::cout << "Consumer is waiting for items...\n";
		ir->cv_not_empty.wait(lck);		//	等待不为空通知
	}
	int data = ir->buff[ir->read_pos++];
	if (ir->read_pos == ITEM_REPOSITORY_SIZE)
	{
		ir->read_pos = 0;
	}

	ir->cv_not_full.notify_one();	//	通知生产者产品库不为满
	lck.unlock();
	
	return data;
}

void ConsumeTask()
{
	static int cnt = 0;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		std::cout << "Consume the " << ConsumeItem(&gItemRepository) << "^th item\n";	//	消费一个产品
		if (++cnt == ITEMS_TO_PRODUCE)	//	如果产品消费个数为 ITEMS_TO_PRODUCE， 完成任务，退出线程
		{
			break;
		}
	}
}