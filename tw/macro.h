#pragma once
#include <assert.h>
#include "util.h"
#include "log.h"
#define TW_ASSERT(x)\
    if(!(x)){\
        TW_LOG_ERROR(TW_LOG_ROOT()) << "ASSERTION: "#x\
        << "\nbacktrace:\n" \
        << tw::BackTraceToString();\
        assert(x);\
    }

#define TW_ASSERT2(x, w)\
    if(!(x)){\
        TW_LOG_ERROR(TW_LOG_ROOT()) << "ASSERTION: "#x\
        << "\n" << w \
        << "\nbacktrace:\n" \
        << tw::BackTraceToString();\
        assert(x);\
    }
