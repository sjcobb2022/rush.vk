#pragma once

#include <vulkan/vulkan.h>

// std
#include <string>
#include <vector>



namespace rush
{
    namespace vkext
    {

        std::vector<const char *> getExtensions();

        void hasGflwRequiredInstanceExtensions();

    }
}