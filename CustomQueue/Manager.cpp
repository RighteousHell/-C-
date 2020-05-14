#include "Manager.h"
#include "Attendee.h"
#include "Printer.h"

#include <map>
#include <mutex>
#include <string>
using namespace std::chrono_literals;

class Message;

Manager::Manager( const std::string&& name_thread, size_t size_of_queue )
    : m_name_of_thread( name_thread ),
    m_queue_of_messages(size_of_queue)
{
    
}


void
Manager::wait_and_handle()
{   
    while( !m_stop )
    {
        while( !m_queue_of_messages.isempty() )
        {
            if( m_stop )
            {
                break;
            }
            Message tmp_msg = m_queue_of_messages.pop();
            if( check_member_existing( tmp_msg.get_receiver_name() ) )
            {
                delivery_msg( tmp_msg );
                //std::cout << m_queue_of_messages.get_size() << " In thread: " << m_name_of_thread <<  std::endl;  // *****Debug trace for  checking state of queue*****
            }
            else
            {
                m_pr.custom_print( "Message for " + tmp_msg.get_receiver_name() + " from " + tmp_msg.get_sender()->get_name() + " can't be delivery! Reason: Receiver was not found! " + "In thread named: " + m_name_of_thread );
            }
            std::this_thread::sleep_for( 50ms );
        }
    }
}

Queue&
Manager::get_queue() 
{
    return m_queue_of_messages;
}


void
Manager::delivery_msg( Message& last_msg )
{
    m_pr.custom_print( "Message for " + last_msg.get_receiver_name() + " was found! Message: " + last_msg.get_sender()->get_msg() + " from " + last_msg.get_sender()->get_name() );
    m_attendees.at( last_msg.get_receiver_name() )->answer( last_msg.get_sender() );
}

bool
Manager::check_member_existing( const std::string& searched_attendee_name ) const
{
    if( m_attendees.end() != m_attendees.find( searched_attendee_name ) )
        return true;
    else
    {
        return false;
    }
}

void
Manager::stop()
{
    m_stop = true;
}

void
Manager::add_new_attendee( std::shared_ptr< Attendee_interface > new_attendee )
{
    m_attendees.insert( {new_attendee->get_name(), new_attendee} );
}

void
Manager::set_attendees_list_for_respond( std::string&& cur_attendee, std::map< std::string, Manager& >&& attendees_for_response )
{
    if( check_member_existing( cur_attendee ) )
    {
        m_attendees.at( cur_attendee )->set_new_attendees_list( attendees_for_response );
    }
}

void
Manager::send_initial_msg( std::string&& sender_name, std::string&& receiver_name, Manager& target_manager )
{
    m_attendees.at( sender_name )->send_msg( receiver_name, target_manager );
}

Manager::~Manager()
{
}