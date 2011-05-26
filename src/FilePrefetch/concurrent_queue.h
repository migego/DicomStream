/*
 * concurrent_queue.h
 *
 *  Created on: May 22, 2011
 *      Author: aboxer
 */

#ifndef CONCURRENT_QUEUE_H_
#define CONCURRENT_QUEUE_H_

#include <list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include "UpDownIterator.h"

template < typename Data, typename Iterator > class concurrent_queue
{
protected:
    std::list<Iterator> the_queue;
    mutable boost::mutex the_mutex;
    boost::condition_variable the_condition_variable;
public:
    void push(Iterator const& iterator)
    {
        boost::mutex::scoped_lock lock(the_mutex);
        the_queue.push_back(iterator);
        lock.unlock();
        the_condition_variable.notify_one();
    }

    bool empty() const
    {
        boost::mutex::scoped_lock lock(the_mutex);
        return the_queue.empty();
    }

    bool try_pop(Data& popped_value)
    {
        boost::mutex::scoped_lock lock(the_mutex);
        if(the_queue.empty())
        {
            return false;
        }

        Iterator iter = the_queue.front();
        while (!iter.nextFragment(popped_value))
            the_queue.pop_front();

        return true;
    }

    void wait_and_pop(Data& popped_value)
    {
        boost::mutex::scoped_lock lock(the_mutex);
        while(the_queue.empty())
        {
            the_condition_variable.wait(lock);
        }

        popped_value=the_queue.front();
        the_queue.pop();
    }

};

#endif /* CONCURRENT_QUEUE_H_ */
