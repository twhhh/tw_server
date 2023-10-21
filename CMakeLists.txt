cmake_minimum_required(VERSION 2.8)
project(test)


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(.)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()


set(SRC 
tw/log.cc 
tw/util.cc
tw/thread.cc
tw/fiber.cc
tw/scheduler.cc
tw/iomanager.cc
tw/timer.cc
tw/fd_manager.cc
tw/hook.cc
tw/address.cc
tw/socket.cc
tw/bytearray.cc
tw/tcp_server.cc
tw/stream.cc
tw/socket_stream.cc
tw/http/http.cc
tw/http/httpclient_parser.rl.cc
tw/http/http11_parser.rl.cc
tw/http/http_parser.cc
tw/http/http_session.cc
tw/http/http_server.cc
)

add_library(tw SHARED ${SRC})

set(LIBS
        tw
        pthread
        dl
        )

add_executable(test test/test.cc)#-o
target_link_libraries(test ${LIBS})#-l

add_executable(test_thread test/test_thread.cc)
target_link_libraries(test_thread ${LIBS})

add_executable(test_scheduler  test/test_scheduler.cc)
target_link_libraries(test_scheduler ${LIBS})

add_executable(test_iomanager  test/test_iomanager.cc)
target_link_libraries(test_iomanager ${LIBS})

add_executable(test_hook  test/test_hook.cc)
target_link_libraries(test_hook ${LIBS})

add_executable(test_address  test/test_address.cc)
target_link_libraries(test_address ${LIBS})

add_executable(test_socket test/test_socket.cc)
target_link_libraries(test_socket ${LIBS})

add_executable(test_bytearray test/test_bytearray.cc)
target_link_libraries(test_bytearray ${LIBS})

add_executable(echo_server example/echo_server.cc)
target_link_libraries(echo_server ${LIBS})

add_executable(test_http test/test_http.cc)
target_link_libraries(test_http ${LIBS})

add_executable(test_http_parser test/test_http_parser.cc)
target_link_libraries(test_http_parser ${LIBS})

add_executable(test_http_server test/test_http_server.cc)
target_link_libraries(test_http_server ${LIBS})


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)