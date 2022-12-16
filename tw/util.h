#pragma once
#include <string>
#include <sys/syscall.h>
#include <unistd.h>
namespace tw{
uint32_t GetFiberID();
pid_t GetThreadID();

std::string BackTraceToString();
}