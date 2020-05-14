#pragma once

#include <string>
#include "Printer.h"

class Manager;
class Attendee_interface;
struct Message
{
public:
    Message( const std::string& receiver_name, Attendee_interface* sender );
    Message()                 = delete;
    Message( const Message& copied_obj ) = default;
    Message( const Message&& replaced_obj );

    ~Message();

    Message& operator=( const Message& ) = default;
    Message& operator=( const Message&& replaced_obj);

    Attendee_interface* get_sender() const;
    const std::string& get_receiver_name() const;

private:
    std::string m_receiver_name;
    Attendee_interface*   p_sender;
};