#include "vkinstance.hpp"

#include <spdlog/spdlog.h>

const char *to_string_message_type(VkDebugUtilsMessageTypeFlagsEXT s)
{
    if (s == 7)
        return "General | Validation | Performance";
    if (s == 6)
        return "Validation | Performance";
    if (s == 5)
        return "General | Performance";
    if (s == 4 /*VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT*/)
        return "Performance";
    if (s == 3)
        return "General | Validation";
    if (s == 2 /*VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT*/)
        return "Validation";
    if (s == 1 /*VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT*/)
        return "General";
    return "Unknown";
}

namespace rush
{



    InstanceBuilder &InstanceBuilder::set_app_name(const char *app_name)
    {
        if (!app_name)
            return *this;
        info.application_name = app_name;
        return *this;
    };

    InstanceBuilder &InstanceBuilder::set_engine_name(const char *engine_name)
    {
        if (!engine_name)
            return *this;
        info.engine_name = engine_name;
        return *this;
    };

    

}
