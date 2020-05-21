cmake_minimum_required (VERSION 3.8)

project (Sender)


add_executable (Sender sender.cpp sender.h)
target_link_libraries (Sender libmbedcrypto.a)
