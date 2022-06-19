#include "pipeline.hpp"

#ifndef ENGINE_DIR
#define ENGINE_DIR "../../../"
#endif

namespace rush
{

    void Pipeline::bindBuffer(VkCommandBuffer buff, VkPipelineBindPoint bindPoint)
    {
        // spdlog::info("{}", shaderStages);
        vkCmdBindPipeline(buff, bindPoint, pipeline);
        // vkCreateGraphicsPipelines()
    }

    Pipeline::operator VkPipeline() const { return this->pipeline; }

    PipelineBuilder::PipelineBuilder(Device device){
        info.device = device;
    }

    // PipelineBuilder::PipelineBuilder(Device device, const std::string &vert_path, const std::string &frag_path){}

    Pipeline PipelineBuilder::build() const
    {
        auto vertCode = read_file("assets/shaders/simple_shader.vert.spv");
        auto fragCode = read_file("assets/shaders/simple_shader.frag.spv");

        auto vertShaderModule = create_shader_module(info.device, vertCode);
        auto fragShaderModule = create_shader_module(info.device, fragCode);

        // VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

        // TODO: like most of the work...
        //  VkGraphicsPipelineCreateInfo pipelineInfo{};
        
        Pipeline pipeline{};

        return pipeline;
    };

    //util stuff
    VkShaderModule PipelineBuilder::create_shader_module(VkDevice device, const std::vector<char> &code) const
    {
        VkShaderModule shaderModule;
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
        createInfo.pNext = nullptr;

        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create shader module");
        }

        return shaderModule;
    }

    std::vector<char> PipelineBuilder::read_file(const std::string &filepath)
    {
        std::string relativePath = ENGINE_DIR + filepath;
        std::ifstream file{relativePath, std::ios::ate | std::ios::binary};

        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file: " + relativePath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }


}