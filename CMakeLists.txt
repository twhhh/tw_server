cmake_minimum_required(VERSION 2.8)
project(test)


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(tw)
include_directories(/apps/sylar/include)#-L
link_directories(/apps/sylar/lib)#-I



set(LIBS
        pthread
        )
set(SRC 
tw/log.cc 
tw/util.cc
tw/thread.cc
tw/fiber.cc
tw/scheduler.cc
)


add_executable(test ${SRC} test.cc)#-o
target_link_libraries(test ${LIBS})#-l

add_executable(test_thread ${SRC} test_thread.cc)
target_link_libraries(test_thread ${LIBS})

add_executable(test_scheduler ${SRC} test_scheduler.cc)
target_link_libraries(test_scheduler ${LIBS})