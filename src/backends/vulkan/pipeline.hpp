#pragma once

#include "rush_pch.hpp"
#include "device.hpp"
#include "shader.hpp"

namespace rush
{

    // struct ShaderModule
    // {
    //     std::vector<uint32_t> code;
    //     VkShaderModule module;
    // };

    // struct ShaderEffect
    // {
    //     VkPipelineLayout layout;

    //     std::array<VkDescriptorSetLayout, 4> setLayouts;

    // private:
    //     struct ShaderStage
    //     {
    //         ShaderModule *shaderModule;
    //         VkShaderStageFlagBits stage;
    //     };

    //     std::vector<ShaderStage> stages;
    // };

    // class ShaderEffectBuilder{}; 

    // struct ShaderPass
    // {
    //     ShaderEffect *effect{nullptr};
    //     VkPipeline pipeline{VK_NULL_HANDLE};
    //     VkPipelineLayout layout{VK_NULL_HANDLE};
    // };

    // struct EffectTemplate{
        
    // };

    struct Pipeline
    { // TODO: Make this into vkblancos pipeline creation thing
        VkPipeline pipeline;
        VkShaderModule vert; //temporary
        VkShaderModule frag; //temporary also...
        //maybe have a vector of the stages.

        void bindBuffer(VkCommandBuffer buff, VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);

        operator VkPipeline() const;
    };

    class PipelineBuilder
    {
    public:
        PipelineBuilder(Device device);

        PipelineBuilder &addStage(ShaderStage stage); // for more expandability

        Pipeline build() const;

    private:
        struct PipelineBuilderInfo
        {
            Device device;
            VkPipelineVertexInputStateCreateInfo vertex_input_state;
            VkGraphicsPipelineCreateInfo pipelineInfo;
            // VkComputePipelineCreateInfo compute_pipeline_create;
        } info;
        
    };

}