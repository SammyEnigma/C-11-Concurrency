/************************************************************************/
/*  
	�������� - ��������ģ����ֻ��һ�������ߺ�һ�������ߣ������߲�ͣ������Ʒ���з����Ʒ��
	��������Ӳ�Ʒ����ȡ�߲�Ʒ����Ʒ���ݻ������ƣ�ֻ������һ����Ŀ�Ĳ�Ʒ�����������������Ʒ���ٶȹ��죬
	����Ҫ�ȴ�������ȡ�߲�Ʒ֮�󣬲�Ʒ�ⲻΪ�ղ��ܼ�������Ʒ���з����µĲ�Ʒ��
	�෴�����������ȡ�߲�Ʒ���ٶȹ��죬��������ٲ�Ʒ����û�в�Ʒ��ʹ�õ������
	��ʱ��Ҫ�ȴ������߷���һ����Ʒ�������߲��ܼ���������C++11ʵ�ֵ������ߵ�������ģ�͵Ĵ������£�                                                                   
*/
/************************************************************************/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

static const int ITEM_REPOSITORY_SIZE = 10;		//	item buffer size
static const int ITEMS_TO_PRODUCE = 1000;		//	number of produce
											
/*	����������	*/
static void ProduceTask();
/*	����������	*/
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
	int buff[ITEM_REPOSITORY_SIZE];				//	��Ʒ�����������read_pos��write_posģ�ͻ��ζ���
	size_t read_pos;							//	�����߶�ȡ��Ʒλ��
	size_t write_pos;							//	������д���Ʒλ��
	std::mutex mtx;								//	��������������Ʒ������
	std::condition_variable cv_not_full;		//	����������ָʾ��Ʒ��������Ϊ��
	std::condition_variable cv_not_empty;		//	����������ָʾ��Ʒ��������Ϊ��

	_tagItemRepository()
	{
		memset(buff, 0, ITEM_REPOSITORY_SIZE);
		read_pos = 0;
		write_pos = 0;
	}
}ItemRepository, *PItemRepository;

ItemRepository gItemRepository;		//	ȫ�ֱ����������ߺ������߲����ñ���

void ProduceItem(PItemRepository ir, int item)
{
	std::unique_lock<std::mutex> lck(ir->mtx);
	if ((ir->write_pos + 1) % ITEM_REPOSITORY_SIZE == ir->read_pos)
	{
		std::cout << "Producer is waiting for an empty slot...\n";
		ir->cv_not_full.wait(lck);	//	�ȴ���Ϊ��֪ͨ
	}

	ir->buff[ir->write_pos++] = item;	//	д���Ʒ
	if (ir->write_pos == ITEM_REPOSITORY_SIZE)	//	д��λ�����Ƕ����������������Ϊ��ʼλ��
	{
		ir->write_pos = 0;
	}

	ir->cv_not_empty.notify_one();	//	֪ͨ�����߲�Ʒ�ⲻΪ��
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
		ir->cv_not_empty.wait(lck);		//	�ȴ���Ϊ��֪ͨ
	}
	int data = ir->buff[ir->read_pos++];
	if (ir->read_pos == ITEM_REPOSITORY_SIZE)
	{
		ir->read_pos = 0;
	}

	ir->cv_not_full.notify_one();	//	֪ͨ�����߲�Ʒ�ⲻΪ��
	lck.unlock();
	
	return data;
}

void ConsumeTask()
{
	static int cnt = 0;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		std::cout << "Consume the " << ConsumeItem(&gItemRepository) << "^th item\n";	//	����һ����Ʒ
		if (++cnt == ITEMS_TO_PRODUCE)	//	�����Ʒ���Ѹ���Ϊ ITEMS_TO_PRODUCE�� ��������˳��߳�
		{
			break;
		}
	}
}