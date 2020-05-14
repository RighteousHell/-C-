#include "Man.h"

Man::Man( const std::string& my_name, const std::string& my_msg, Manager& my_manager )
    : Attendee_interface( my_name, my_msg, my_manager )
{
}

void
Man::respond( Attendee_interface* sender )
{
    //*****Gentleman should keep silent*****
}