#include "Message.h"

#include <string>


Message::Message( const std::string& receiver_name, Attendee_interface* sender )
    : m_receiver_name( receiver_name ), p_sender( sender )
{
}


Message&
Message::operator=( const Message&& replaced_obj )
{
    if( &replaced_obj == this )
    {
        return *this;
    }
    m_receiver_name = replaced_obj.m_receiver_name;
    p_sender        = replaced_obj.p_sender;
    return *this;
}

Attendee_interface*
Message::get_sender() const
{
    return p_sender;
}

const std::string&
Message::get_receiver_name() const
{
    return m_receiver_name;
}

Message::Message( const Message&& replaced_obj )
{
    m_receiver_name = replaced_obj.m_receiver_name;
    p_sender        = replaced_obj.p_sender;
}

Message::~Message()
{
}
