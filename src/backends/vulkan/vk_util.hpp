#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace rush
{
    template <typename T>
    void setup_pNext_chain(T &structure, std::vector<VkBaseOutStructure *> const &structs)
    {
        structure.pNext = nullptr;
        if (structs.size() <= 0)
            return;
        for (size_t i = 0; i < structs.size() - 1; i++)
        {
            structs.at(i)->pNext = structs.at(i + 1);
        }
        structure.pNext = structs.at(0);
    }
}
