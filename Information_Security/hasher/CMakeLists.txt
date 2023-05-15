cmake_minimum_required(VERSION 3.17)
project(stream_chain_hasher)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g ")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

find_package(OpenSSL REQUIRED)

if(OPENSSL_FOUND)
  include_directories(${OPENSSL_INCLUDE_DIRS})
  message(STATUS "OpenSSL Found!")
endif()

add_executable(hasher main.cpp)
target_link_libraries(hasher OpenSSL::Crypto OpenSSL::SSL)
