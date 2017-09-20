/****************************************************************************************************/
/*   与单生产者和单消费者模型不同的是，单生产者-多消费者模型中可以允许多个消费者同时从产品库中取走产品。		*/
/*	 所以除了保护产品库在多个读写线程下互斥之外，还需要维护消费者取走产品的计数器                            */
/****************************************************************************************************/

#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <atomic>

static const int ITEM_REPOSITORY_SIZE = 4;		//	item buffer size
static const int ITEMS_TO_PRODUCE = 10;		//	number of produce

												/*	生产者任务	*/
static void ProduceTask();
/*	消费者任务	*/
static void ConsumeTask();

int main(int argc, char* argv[])
{
	std::thread producer(ProduceTask);
	std::vector<std::thread> consumer;
	for (int i = 0; i < 5; ++i)
	{
		consumer.push_back(std::thread(ConsumeTask));
	}

	producer.join();	
	for (auto& x : consumer)
	{
		x.join();
	}

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
		std::cout << "Producer is waiting for a empty slot...\n";
		ir->cv_not_full.wait(lck);
	}
	ir->buff[ir->write_pos++] = item;
	if (ir->write_pos == ITEM_REPOSITORY_SIZE)
	{
		ir->write_pos = 0;
	}

	ir->cv_not_empty.notify_all();
	lck.unlock();
}

void ProduceTask()
{
	for (int i = 1; i <= ITEMS_TO_PRODUCE; ++i)
	{
		std::cout << "Producer thread " << std::this_thread::get_id() << " producing the " << i << "^th item\n";
		ProduceItem(&gItemRepository, i);
	}
	std::cout << "Producer thread " << std::this_thread::get_id() << " is exiting...\n";
}

int ConsumeItem(PItemRepository ir)
{
	std::unique_lock<std::mutex> lck(ir->mtx);
	while (ir->read_pos == ir->write_pos)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		std::cout << "Consumer thread[" << std::this_thread::get_id() << "] is waiting for not empty...\n";
		ir->cv_not_empty.wait(lck);
	}
	int data = ir->buff[ir->read_pos++];
	if (ir->read_pos == ITEM_REPOSITORY_SIZE)
	{
		ir->read_pos = 0;
	}

	ir->cv_not_full.notify_one();
	lck.unlock();

	return data;
}

std::atomic_uint16_t cnt(0);
void ConsumeTask()
{
	while (true)
	{
		if (++cnt > ITEMS_TO_PRODUCE)
		{
			std::cout << "Consumer thread " << std::this_thread::get_id() << " is exiting...\n";
			break;
		}
		std::cout << "Consumer thread " << std::this_thread::get_id() << " consuming the " << ConsumeItem(&gItemRepository) << "^th item\n";
	}
}