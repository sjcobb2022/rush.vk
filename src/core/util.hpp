#pragma once

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.h>

#include <map>
#include <functional>
#include <string>


// <-- MACROS -->
#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

namespace rush{

    namespace util{

    }
}