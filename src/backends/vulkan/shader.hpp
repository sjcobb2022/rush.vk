#pragma once

#include "rush_pch.hpp"
#include "device.hpp"
#include <shaderc/shaderc.hpp>

namespace rush
{

    struct ShaderModule
    {
        std::vector<uint32_t> code; //needed for reflection --> TODO: make reflection part of shader instead of pipeline? depends
        VkShaderModule module;
    };

    struct ShaderStage
    {
        ShaderModule *shaderModule;
        VkShaderStageFlagBits stage;
    };

    class ShaderBuilder
    {
    public:
        ShaderBuilder(Device device);
        ShaderBuilder(Device device, std::string &filepath, VkShaderStageFlagBits stage);

        ShaderBuilder &set_path(std::string filepath);

        ShaderBuilder &set_stage(VkShaderStageFlagBits stage);

        ShaderStage build();

    private:
        const std::vector<uint32_t> read_file(const std::string &filepath, VkShaderStageFlagBits stage);
        VkShaderModule create_shader_module(VkDevice device, const std::vector<uint32_t> &code) const;

        struct ShaderInfo
        {
            Device device;
            VkShaderStageFlagBits stage;
            std::string filepath;
        } info;
    };

}