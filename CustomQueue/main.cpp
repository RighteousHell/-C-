#include "ExitCodes.h"
#include "Man.h"
#include "Manager.h"
#include "Queue.h"
#include "Woman.h"
#include "Attendee.h"

#include <map>


namespace
{
void
Talking( Manager& controler )
{
    controler.wait_and_handle();
}

void
initial_attendees( Manager& manager, Manager& manager1, Manager& manager2 )
{
    manager.add_new_attendee( {std::make_shared< Man >( Man( "Johnny", "Hello! I'm Johnny", manager ) )} );
    manager.add_new_attendee( {std::make_shared< Woman >( Woman( "Ketty", "Hello! I'm Ketty", manager ) )} );
    manager.add_new_attendee( {std::make_shared< Man >( Man( "Luci", "Hello! I'm Luci", manager ) )} );
    manager.add_new_attendee( {std::make_shared< Attendee >( Attendee( "Kensin", "Hello! I'm Kensin", manager ) )} );

    manager1.add_new_attendee( {std::make_shared< Man >( Man( "Tonny", "Hello! I'm Tonny", manager1 ) )} );
    manager1.add_new_attendee( {std::make_shared< Woman >( Woman( "Lazy", "Hello! I'm Lazy", manager1 ) )} );
    manager1.add_new_attendee( {std::make_shared< Woman >( Woman( "Karla", "Hello! I'm Karla", manager1 ) )} );
    manager1.add_new_attendee( {std::make_shared< Attendee >( Attendee( "Kaoru", "Hello! I'm Kaoru", manager1 ) )} );

    manager2.add_new_attendee( {std::make_shared< Man >( Man( "Igor", "Hello! I'm Igor", manager2 ) )} );
    manager2.add_new_attendee( {std::make_shared< Man >( Man( "Stepa", "Hello! I'm Stepa", manager2 ) )} );
    manager2.add_new_attendee( {std::make_shared< Woman >( Woman( "Tamara", "Hello! I'm Tamara", manager2 ) )} );
    manager2.add_new_attendee( {std::make_shared< Attendee >( Attendee( "Satoshi", "Hello! I'm Satoshi", manager2 ) )} );
}

void
initial_addressats_lists( Manager& manager, Manager& manager1, Manager& manager2 )
{
    manager.set_attendees_list_for_respond( "Ketty", {{"Tamara", manager2}, {"Karla", manager1}} );
    manager1.set_attendees_list_for_respond( "Karla", {{"Lazy", manager1}, {"Ketty", manager}} );
    manager2.set_attendees_list_for_respond( "Tamara", {{"Igor", manager2}, {"Johnny", manager}} );
}

void
initial_filling_queueues( Manager& manager, Manager& manager1, Manager& manager2 )
{
    manager2.send_initial_msg( "Igor", "Karla", manager1 );
    manager.send_initial_msg( "Ketty", "Igor", manager2 );
    manager.send_initial_msg( "Luci", "Johnny", manager );
    manager1.send_initial_msg( "Lazy", "Kensin", manager );

    manager1.send_initial_msg( "Tonny", "Lazy", manager1 );
    manager1.send_initial_msg( "Tonny", "Tamara", manager2 );
    manager1.send_initial_msg( "Karla", "Stepa", manager2 );
    manager2.send_initial_msg( "Satoshi", "Kaoru", manager1 );

    manager2.send_initial_msg( "Igor", "Johnny", manager );
    manager2.send_initial_msg( "Stepa", "Karla", manager1 );
    manager2.send_initial_msg( "Tamara", "Igor", manager2 );
    manager1.send_initial_msg( "Karla", "Putin", manager );  //******message for not exist user*****
    manager1.send_initial_msg( "Tonny", "Ketty", manager );
    manager1.send_initial_msg( "Tonny", "Stepa", manager2 );
    manager.send_initial_msg( "Ketty", "Tamara", manager2 );
    manager1.send_initial_msg( "Tonny", "Satoshi", manager2);
}
}  // namespace


int
main( int arg, char* argv[] )
{
    Manager manager( "Thread1", 50 );
    Manager manager1( "Thread2", 50 );
    Manager manager2( "Thread3", 50 );


    initial_attendees( manager, manager1, manager2 );

    initial_addressats_lists( manager, manager1, manager2 );

    initial_filling_queueues( manager, manager1, manager2 );

    std::thread thread3( Talking, std::ref( manager ) );
    std::thread thread1( Talking, std::ref( manager1 ) );
    std::thread thread2( Talking, std::ref( manager2 ) );

    getchar();  // wait for press Enter
    manager.stop();
    manager1.stop();
    manager2.stop();

    if( thread1.joinable() )
    {
        thread1.join();
    }

    if( thread2.joinable() )
    {
        thread2.join();
    }

    if( thread3.joinable() )
    {
        thread3.join();
    }

    return ExitCodes::OK_EXIT;
}
