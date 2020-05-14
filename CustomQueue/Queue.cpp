#include "Queue.h"

Queue::Queue( size_t cap ) 
    : m_capacity( cap )
{
}

Message
Queue::pop()
{
    std::lock_guard< std::mutex > lk( m_queue_mutex );
    Message tmp_msg = m_list.front();
    m_list.pop();
    return tmp_msg;
}

void
Queue::push( Message&& sent_msg )
{
    std::lock_guard< std::mutex > lk( m_queue_mutex );
    if( m_list.size() < m_capacity )
    {
        m_list.push(sent_msg);
    }
    else
    {
        throw std::length_error("Error! Queue is full");
    }
}

size_t
Queue::get_size() const
{
    std::lock_guard< std::mutex > lk( m_queue_mutex );
    return m_list.size();
}


bool
Queue::isempty() const
{
    std::lock_guard< std::mutex > lk( m_queue_mutex );
    return m_list.empty();
}