#include "Printer.h"

#include <iostream>

std::mutex Printer::m_print_mutex;

void
Printer::custom_print( const std::string& print_data )
{
    std::lock_guard< std::mutex > lkp( Printer::m_print_mutex );
    std::cout << print_data << std::endl;
}
