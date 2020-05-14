#pragma once
#include <mutex>


class Printer
{


public:
    Printer() = default;
    ~Printer() = default;
    void
    custom_print( const std::string& print_data );

private:
    static std::mutex m_print_mutex;
};