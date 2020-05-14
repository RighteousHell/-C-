#pragma once

#include "Queue.h"
#include <atomic>
#include <map>


class Attendee_interface;
class Printer;

class Manager
{
public:
    Manager() = delete;
    Manager( const std::string&& manager_name, size_t size_of_queue );
    Manager( const Manager& )  = delete;
    Manager( const Manager&& ) = delete;

    ~Manager();

    Manager& operator=( Manager& ) = delete;
    Manager& operator=( Manager&& ) = delete;

    Queue&  get_queue();
    void    wait_and_handle();
    void    delivery_msg( Message& last_msg );
    void    stop();
    void    add_new_attendee( std::shared_ptr< Attendee_interface > new_attendee );
    void    set_attendees_list_for_respond( std::string&& cur_attendee, std::map< std::string, Manager& >&& attendees_for_response);
    void    send_initial_msg( std::string&& sender_name, std::string&& receiver_name, Manager& target_manager );
    bool    check_member_existing( const std::string& searched_attendee_name ) const;

private:
    using Attendee_type = std::map< std::string, std::shared_ptr< Attendee_interface > >;
    Queue           m_queue_of_messages;
    Attendee_type    m_attendees;
    std::atomic_bool m_stop;
    std::string      m_name_of_thread;
    Printer          m_pr;
};