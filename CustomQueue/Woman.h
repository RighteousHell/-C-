#pragma once

#include "Attendee_interface.h"

class Woman : public Attendee_interface
{
private:
public:
    Woman() = delete;
    Woman( const std::string& my_name, const std::string& my_msg, Manager& my_manager );
    Woman( const Woman& copied_obj ) = default;
    Woman( Woman&& replaced_obj )    = default;
    Woman& operator=( const Woman& ) = default;
    Woman& operator=( Woman&& ) = default;
    ~Woman() override =default;


private:
    void respond( Attendee_interface* sender ) override;
};