#pragma once 

#include "Attendee_interface.h"

class Man : public Attendee_interface
{
public:
    Man()       = delete;
    Man( const std::string& my_name, const std::string& my_msg, Manager& my_manager );
    Man( const Man& copied_obj ) = default;
    Man( Man&& replaced_obj )      = default;
    Man& operator=( const Man& ) = default;
    Man& operator=( Man&& ) = default;
    ~Man() override = default;

private:
    void respond( Attendee_interface* sender ) override;
};