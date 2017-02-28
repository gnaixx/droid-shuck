cmake_minimum_required(VERSION 3.4.1)

add_library(shuck SHARED src/main/cpp/Shuck.cpp src/main/cpp/Common.cpp)

find_library(log-lib log )

find_library(android-lib android)

target_link_libraries( shuck ${log-lib} ${android-lib})
