/*
 * Queue.cpp
 *
 *  Created on: May 26, 2014
 *      Author: cawl-mobile
 */

#include "Queue.h"

namespace Db {

Queue::Queue() {
	// TODO Auto-generated constructor stub

}

Queue::~Queue() {
	// TODO Auto-generated destructor stub
}

} /* namespace Db */


template<typename Data>
class concurrent_queue
{
private:
    std::queue<Data> the_queue;
    mutable boost::mutex the_mutex;
public:
    void push(const Data& data)
    {
        boost::mutex::scoped_lock lock(the_mutex);
        the_queue.push(data);
    }

    bool empty() const
    {
        boost::mutex::scoped_lock lock(the_mutex);
        return the_queue.empty();
    }

    Data& front()
    {
        boost::mutex::scoped_lock lock(the_mutex);
        return the_queue.front();
    }

    Data const& front() const
    {
        boost::mutex::scoped_lock lock(the_mutex);
        return the_queue.front();
    }

    void pop()
    {
        boost::mutex::scoped_lock lock(the_mutex);
        the_queue.pop();
    }
};
