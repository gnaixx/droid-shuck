cmake_minimum_required(VERSION 3.4.1)

add_library( shuck SHARED src/main/cpp/Shuck.cpp )

find_library( log-lib log )

target_link_libraries( shuck ${log-lib} )
