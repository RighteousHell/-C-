#include "Attendee.h"

#include "Manager.h"
#include <string>

Attendee::Attendee( const std::string& my_name, const std::string& my_msg, Manager& my_manager )
    : Attendee_interface( my_name, my_msg, my_manager )
{
}

void
Attendee::respond( Attendee_interface* sender )
{
    send_msg( sender->get_name(), sender->get_thread() );
}

