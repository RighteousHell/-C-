#pragma once



#include "Message.h"

#include <mutex>
#include <queue>


class Queue
{
public:
    explicit Queue( size_t cap = 50);
    Queue() = delete;
    Queue( const Queue& )= delete;
    Queue( const Queue&& ) = delete;

    ~Queue() = default;

    Queue& operator=( const Queue& ) = delete;

    bool   isempty() const;
    size_t get_size() const;

    Message pop();
    void    push( Message&& sent_msg);

private:
    size_t                m_capacity = 50;
    std::queue< Message > m_list;
    mutable std::mutex    m_queue_mutex;
};