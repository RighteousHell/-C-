cmake_minimum_required (VERSION 3.8)

project (Receiver)


add_executable (Receiver receiver.cpp)
target_link_libraries (Receiver libmbedcrypto.a)
