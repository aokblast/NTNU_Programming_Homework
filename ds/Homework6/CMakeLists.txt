cmake_minimum_required(VERSION 3.20)
project(Homework6)

set(CMAKE_CXX_STANDARD 14)


include_directories(src/lib/pdcurses)
add_executable(Homework6  src/LinkedList.cpp src/LinkedList.h src/main.cpp src/ui.cpp src/UI.h src/Card.cpp src/Card.h)
target_link_libraries(Homework6 PUBLIC "${PROJECT_SOURCE_DIR}/src/lib/pdcurses/pdcurses.dll")