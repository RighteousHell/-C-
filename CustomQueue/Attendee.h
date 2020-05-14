#pragma once

#include <map>
#include <string>
#include "Attendee_interface.h"

class Manager;
class Attendee : public Attendee_interface
{
public:
    using Attendees_List = std::map< std::string, Manager& >;
    Attendee()              = delete;
    Attendee( const std::string& my_name, const std::string& my_msg, Manager& my_manager );
    Attendee( const Attendee& copied_obj ) = default;
    Attendee( Attendee&& replaced_obj )    = default;
    Attendee& operator=( const Attendee& ) = default;
    Attendee& operator=( Attendee&& ) = default;
    ~Attendee() override           = default;


private:
    void respond( Attendee_interface* sender ) override;
};