#include "Woman.h"

Woman::Woman( const std::string& my_name, const std::string& my_msg, Manager& my_manager )
    : Attendee_interface( my_name, my_msg,my_manager )
{
}


void
Woman::respond( Attendee_interface* sender )
{
        send_msg( sender->get_name(), sender->get_thread() );

        for( auto& attendee_from_list : sender->get_list_of_attendees() )
        {
            send_msg( attendee_from_list.first, attendee_from_list.second );
        }
}
