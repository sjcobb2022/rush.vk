#pragma once

#include "rush_pch.hpp"
#include "device.hpp"
#include "shader.hpp"
#include "vk_util.hpp"

namespace rush
{
    struct ReflectedBinding
    {
        uint32_t set;
        uint32_t binding;
        VkDescriptorType type;
    };

    struct Pipeline
    {
        VkPipeline pipeline;
        VkPipelineLayout layout;
        std::unordered_map<std::string, ReflectedBinding> bindings;
        std::array<VkDescriptorSetLayout, 4> setLayouts;

        void bindBuffer(VkCommandBuffer buff, VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);

        operator VkPipeline() const;
    };

    class PipelineBuilder
    {
    public:
        PipelineBuilder(Device device);

        PipelineBuilder &add_stage(ShaderStage stage); // for more expandability

        Pipeline build(VkRenderPass pass);

    private:
        struct PipelineBuilderInfo
        {
            Device device = {VK_NULL_HANDLE};
            VkPipelineLayout builtLayout = {VK_NULL_HANDLE};
            VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
            VkCullModeFlags cullMode = VK_CULL_MODE_NONE;
            VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE;
            VkCompareOp depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
            VkLogicOp logicOp = VK_LOGIC_OP_COPY;
        } info;

        struct DescriptorSetLayoutData
        {
            uint32_t set_number;
            VkDescriptorSetLayoutCreateInfo create_info;
            std::vector<VkDescriptorSetLayoutBinding> bindings;
        };

        struct ReflectionOverrides
        {
            const char *name;
            VkDescriptorType overridenType;
        };

        std::vector<ShaderStage> stages;
        std::unordered_map<std::string, ReflectedBinding> bindings;
        std::array<VkDescriptorSetLayout, 4> setLayouts;

        void reflect_layout(VkDevice device, ReflectionOverrides *overrides, int overrideCount);

        VkPipelineLayoutCreateInfo populate_pipeline_layout_create_info();

        VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info();

        std::vector<VkPipelineShaderStageCreateInfo> PipelineBuilder::get_pipeline_stage_info();
    };

}