#include "Attendee_interface.h"

#include "Manager.h"

#include <string>

Attendee_interface::Attendee_interface( const std::string& my_name, const std::string& my_msg, Manager& my_manager )
    : m_name( my_name ),
    m_msg( my_msg ),
    r_my_manager( my_manager )
{
}


Attendee_interface::~Attendee_interface()
{
}


const std::map< std::string, Manager& >&
Attendee_interface::get_list_of_attendees() const
{
    return m_list_of_attendees;
}

const std::string&
Attendee_interface::get_name() const
{
    return m_name;
}

const std::string&
Attendee_interface::get_msg() const
{
    return m_msg;
}

Manager&
Attendee_interface::get_thread() const
{
    return r_my_manager;
}

void
Attendee_interface::send_msg( const std::string& dest_name, Manager& target_manager )
{
    try
    {
        target_manager.get_queue().push( {dest_name, this} );
    }
    catch( std::length_error& e )
    {
        pr.custom_print( e.what() );
    }
}

void
Attendee_interface::answer( Attendee_interface* sender )
{
    respond( sender );
}

void
Attendee_interface::set_new_attendees_list( Attendees_List& new_list )
{
    m_list_of_attendees = new_list;
}
