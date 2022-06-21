#include "shader.hpp"

#ifndef ENGINE_DIR
#define ENGINE_DIR "../../../"
#endif

namespace rush
{
    ShaderBuilder::ShaderBuilder(Device device)
    {
        info.device = device;
    }

    ShaderBuilder::ShaderBuilder(Device device, std::string &filepath, VkShaderStageFlagBits stage)
    {
        info.device = device;
        info.filepath = filepath;
        info.stage = stage;
    }

    ShaderStage ShaderBuilder::build(){
        ShaderStage shaderStage;

        const auto shader_code = read_file(info.filepath);

        VkShaderModule vk_shader_module = create_shader_module(info.device, shader_code);

        ShaderModule shader_module{shader_code, vk_shader_module};

        return ShaderStage{&shader_module, info.stage};
    }


    ShaderBuilder &ShaderBuilder::set_path(std::string filepath)
    {
        info.filepath = filepath;
        return *this;
    }

    ShaderBuilder &ShaderBuilder::set_stage(VkShaderStageFlagBits stage)
    {
        info.stage = stage;
        return *this;
    }

    std::vector<uint32_t> ShaderBuilder::read_file(const std::string &filepath)
    {
        std::string relativePath = ENGINE_DIR + filepath;
        std::ifstream file{relativePath, std::ios::ate | std::ios::binary};

        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file: " + relativePath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
        file.seekg(0);
        file.read((char*)buffer.data(), fileSize); //read as char* since that is what the file is. Will read the uint32_t into the buffer instead. cred vkblanco
        file.close();
        return buffer;
    }

    VkShaderModule ShaderBuilder::create_shader_module(VkDevice device, const std::vector<uint32_t> &code) const
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size() * sizeof(uint32_t);
        createInfo.pCode = code.data();
        createInfo.pNext = nullptr;

        VkShaderModule shaderModule;

        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create shader module");
        }

        return shaderModule;
    }

}