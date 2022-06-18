#pragma once

#include "rush_pch.hpp"
#include "device.hpp"

namespace rush
{

    struct ShaderModule
    {
        std::vector<uint32_t> code;
        VkShaderModule module;
    };

    struct ShaderEffect
    {
        VkPipelineLayout layout;

        std::array<VkDescriptorSetLayout, 4> setLayouts;

    private:
        struct ShaderStage
        {
            ShaderModule *shaderModule;
            VkShaderStageFlagBits stage;
        };

        std::vector<ShaderStage> stages;
    };

    class ShaderEffectBuilder{}; 

    struct ShaderPass
    {
        ShaderEffect *effect{nullptr};
        VkPipeline pipeline{VK_NULL_HANDLE};
        VkPipelineLayout layout{VK_NULL_HANDLE};
    };

    struct Pipeline
    { // TODO: Make this into vkblancos pipeline creation thing
        VkPipeline pipeline;
        VkShaderModule vert;
        VkShaderModule frag;

        void bindBuffer(VkCommandBuffer buff, VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);

        operator VkPipeline() const;
    };

    class PipelineBuilder
    {
    public:
        PipelineBuilder(Device device);

        PipelineBuilder &addStage(const std::string &path, VkShaderStageFlagBits stage = VK_SHADER_STAGE_VERTEX_BIT); // for more expandability

        Pipeline build() const;

    private:
        static std::vector<char> read_file(const std::string &filepath);

        VkShaderModule create_shader_module(VkDevice device, const std::vector<char> &code) const;

        struct PipelineBuilderInfo
        {
            Device device;
            VkPipelineVertexInputStateCreateInfo vertex_input_state;
            VkGraphicsPipelineCreateInfo pipelineInfo;
            // VkComputePipelineCreateInfo compute_pipeline_create;
        } info;

        // std::vector<ShaderStage> stages;

        // struct PipelineInfo
        // {
        //     VkStructureType sType;
        //     const void *pNext;
        //     VkPipelineCreateFlags flags;
        //     uint32_t stageCount;
        //     const VkPipelineShaderStageCreateInfo*pStages;
        //     const VkPipelineVertexInputStateCreateInfo *pVertexInputState;
        //     const VkPipelineInputAssemblyStateCreateInfo *pInputAssemblyState;
        //     const VkPipelineTessellationStateCreateInfo *pTessellationState;
        //     const VkPipelineViewportStateCreateInfo *pViewportState;
        //     const VkPipelineRasterizationStateCreateInfo *pRasterizationState;
        //     const VkPipelineMultisampleStateCreateInfo *pMultisampleState;
        //     const VkPipelineDepthStencilStateCreateInfo *pDepthStencilState;
        //     const VkPipelineColorBlendStateCreateInfo *pColorBlendState;
        //     const VkPipelineDynamicStateCreateInfo *pDynamicState;
        //     VkPipelineLayout layout;
        //     VkRenderPass renderPass;
        //     uint32_t subpass;
        //     VkPipeline basePipelineHandle;
        //     int32_t basePipelineIndex;
        // } info;
    };

}