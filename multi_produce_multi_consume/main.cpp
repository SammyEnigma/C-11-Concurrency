/************************************************************************/
/* ��ģ�Ϳ���˵��ǰ������ģ�͵��ۺϣ�������Ҫά���������������ֱ�����������������Ʒ����Ŀ����������ȡ�߲�Ʒ����Ŀ��   */                                                  
/* ����Ҳ��Ҫ������Ʒ���ڶ�������ߺͶ�������߻���ط��ʡ�               */
/************************************************************************/

#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <atomic>

static const int ITEM_REPOSITORY_SIZE = 4;		//	item buffer size
static const int ITEMS_TO_PRODUCE = 20;		//	number of produce

											/*	����������	*/
static void ProduceTask();
/*	����������	*/
static void ConsumeTask();

int main(int argc, char* argv[])
{
	std::vector<std::thread> producer;
	std::vector<std::thread> consumer;
	for (int i = 0; i < 4; ++i)
	{
		producer.push_back(std::thread(ProduceTask));
		consumer.push_back(std::thread(ConsumeTask));
	}

	for (auto& x : producer)
	{
		x.join();
	}
	for (auto& x : consumer)
	{
		x.join();
	}

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

	size_t write_cnt;							//	���������ɵ�����
	std::mutex mtx_cnt;							//	���������������������ɵ�����

	_tagItemRepository()
	{
		memset(buff, 0, ITEM_REPOSITORY_SIZE);
		read_pos = 0;
		write_pos = 0;
		write_cnt = 0;
	}
}ItemRepository, *PItemRepository;

ItemRepository gItemRepository;		//	ȫ�ֱ����������ߺ������߲����ñ���

void ProduceItem(PItemRepository ir, int item)
{
	std::unique_lock<std::mutex> lck(ir->mtx);
	while ((ir->write_pos + 1) % ITEM_REPOSITORY_SIZE == ir->read_pos)
	{
		std::cout << "Producer thread " << std::this_thread::get_id() << " is waiting for a empty slot...\n";
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
	while (true)
	{
		std::unique_lock<std::mutex> lck(gItemRepository.mtx_cnt);
		if (++gItemRepository.write_cnt > ITEMS_TO_PRODUCE)
		{
			break;
		}
		int i = gItemRepository.write_cnt;
		lck.unlock();
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

	ir->cv_not_full.notify_all();
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