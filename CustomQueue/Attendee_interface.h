#pragma once

#include "Printer.h"

#include <map>
#include <string>
class Manager;
class Attendee_interface
{
public:
    using Attendees_List = std::map< std::string, Manager& >;
    Attendee_interface( const std::string& my_name, const std::string& my_msg, Manager& my_manager );
    Attendee_interface() = delete;

    virtual ~Attendee_interface();

    const std::string&    get_name() const;
    const std::string&    get_msg() const;
    Manager&              get_thread() const;
    const Attendees_List& get_list_of_attendees() const;

    void set_new_attendees_list( Attendees_List& new_list );

    void send_msg( const std::string& dest_name, Manager& target_manager );
    void answer( Attendee_interface* sender );


protected:
    Attendees_List m_list_of_attendees;
    std::string    m_name;
    std::string    m_msg;
    Manager&       r_my_manager;
    Printer        pr;

private:
    virtual void respond( Attendee_interface* sender ) = 0;
};