cmake_minimum_required(VERSION 3.10)

project(noop LANGUAGES CXX)

add_executable(noop main.cpp)
install(TARGETS noop
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
