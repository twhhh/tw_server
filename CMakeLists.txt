cmake_minimum_required(VERSION 2.8)
project(test)


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(tw)
include_directories(/apps/sylar/include)#-L
link_directories(/apps/sylar/lib)#-I



set(LIBS
        pthread
        dl
        )
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


add_executable(test ${SRC} test.cc)#-o
target_link_libraries(test ${LIBS})#-l

add_executable(test_thread ${SRC} test_thread.cc)
target_link_libraries(test_thread ${LIBS})

add_executable(test_scheduler ${SRC} test_scheduler.cc)
target_link_libraries(test_scheduler ${LIBS})

add_executable(test_iomanager ${SRC} test_iomanager.cc)
target_link_libraries(test_iomanager ${LIBS})

add_executable(test_hook ${SRC} test_hook.cc)
target_link_libraries(test_hook ${LIBS})

add_executable(test_address ${SRC} test_address.cc)
target_link_libraries(test_address ${LIBS})

add_executable(test_socket ${SRC} test_socket.cc)
target_link_libraries(test_socket ${LIBS})

add_executable(test_bytearray ${SRC} test_bytearray.cc)
target_link_libraries(test_bytearray ${LIBS})

add_executable(echo_server ${SRC} example/echo_server.cc)
target_link_libraries(echo_server ${LIBS})

add_executable(test_http ${SRC} test_http.cc)
target_link_libraries(test_http ${LIBS})

add_executable(test_http_parser ${SRC} test_http_parser.cc)
target_link_libraries(test_http_parser ${LIBS})

add_executable(test_http_server ${SRC} test_http_server.cc)
target_link_libraries(test_http_server ${LIBS})