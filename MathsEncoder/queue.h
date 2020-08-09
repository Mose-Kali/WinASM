#include <stdio.h>

#include <queue>

#include <WinSock2.h>
template<typename Data>

class concurrent_queue
{
private:
	int _size;
	struct queue_block
	{
		Data q[0x40];
		unsigned short head, tail;
		queue_block *next;
		queue_block() { head = tail = 0; next = NULL; }
	};
	queue_block *head, *tail;
	mutable boost::mutex the_mutex;
public:
	concurrent_queue() { head = tail = NULL; }
	~concurrent_queue()
	{
		while (head)
		{
			queue_block *p = head;
			head = head->next;
			delete p;
		}
	}
	void push(const Data& data)
	{
		boost::mutex::scoped_lock lock(the_mutex);
		if (!head)
			head = tail = new queue_block;
		if (((tail->tail + 1) & 0x3f) == tail->head)
		{
			tail->next = new queue_block;
			tail = tail->next;
		}
		tail->q[tail->tail] = data;
		tail->tail = (tail->tail + 1) & 0x3f;
		_size++;
	}
	bool empty() const
	{
		boost::mutex::scoped_lock lock(the_mutex);
		return head == NULL;
	}
	Data& front()
	{
		boost::mutex::scoped_lock lock(the_mutex);
		return head->q[head->head];
	}
	Data const& front() const
	{
		boost::mutex::scoped_lock lock(the_mutex);
		return head->q[head->head];
	}
	void pop()
	{
		boost::mutex::scoped_lock lock(the_mutex);
		head->head = (head->head + 1) & 0x3f;
		if (head->head == head->tail)
		{
			queue_block *p = head;
			head = head->next;
			delete p;
		}
		_size--;
	}
	int size()
	{
		boost::mutex::scoped_lock lock(the_mutex);
		return _size;
	}
};