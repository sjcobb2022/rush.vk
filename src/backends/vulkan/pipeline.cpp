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

    PipelineBuilder::PipelineBuilder(Device device)
    {
        info.device = device;
    }

    VkPipelineVertexInputStateCreateInfo PipelineBuilder::vertex_input_state_create_info()
    {
        VkPipelineVertexInputStateCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        info.pNext = nullptr;

        // no vertex bindings or attributes
        info.vertexBindingDescriptionCount = 0;
        info.vertexAttributeDescriptionCount = 0;
        return info;
    }

    PipelineBuilder& PipelineBuilder::add_stage(ShaderStage stage){
        stages.push_back(stage);
        return *this;
    }

    std::vector<VkPipelineShaderStageCreateInfo> PipelineBuilder::get_pipeline_stage_info()
    {

        std::vector<VkPipelineShaderStageCreateInfo> stage_info;
        // stages.clear(); //in case stages is being reused. nervermind why would I clear the stages?? maybe make this another func
        for (auto &s : stages)
        {
            VkPipelineShaderStageCreateInfo shader_create_info{};
            shader_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shader_create_info.pNext = nullptr;
            shader_create_info.stage = s.stage;
            shader_create_info.module = s.shaderModule->module;
            shader_create_info.pName = "main"; // entry point in shader func
            stage_info.push_back(shader_create_info);
        }

        return stage_info;
    }

    Pipeline PipelineBuilder::build(VkRenderPass pass)
    {
        const auto vertexDescription = Vertex::get_vertex_description();

        // vertexInputInfo
        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.pNext = nullptr;
        vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
        vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)vertexDescription.attributes.size();
        vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
        vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)vertexDescription.bindings.size();

        // input assembly
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.pNext = nullptr;
        inputAssembly.topology = info.topology; // hard code for now

        // rasterizer
        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.pNext = nullptr;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = info.polygonMode;
        rasterizer.lineWidth = 1.0f;
        // pip_config.rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.cullMode = info.cullMode; // don't cull for now
        rasterizer.frontFace = info.frontFace;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;
        rasterizer.depthBiasClamp = 0.0f;
        rasterizer.depthBiasSlopeFactor = 0.0f;

        // depth stencil
        VkPipelineDepthStencilStateCreateInfo depthStencil = {};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.pNext = nullptr;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = info.depthCompareOp;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // optional
        depthStencil.maxDepthBounds = 1.0f; // optional
        depthStencil.stencilTestEnable = VK_FALSE;

        // multispample
        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.pNext = nullptr;
        multisampling.sampleShadingEnable = VK_FALSE;
        // 1 sample per shading (techically no sampling)
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;
        multisampling.pSampleMask = nullptr;
        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;

        VkViewport viewport;
        VkRect2D scissor;

        // viewport state
        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.pNext = nullptr;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        // colorblending
        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.pNext = nullptr;

        VkPipelineColorBlendAttachmentState colorBlendAttachments;

        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = info.logicOp;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachments;

        auto const shaderStages = get_pipeline_stage_info();

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.pNext = nullptr;

        pipelineInfo.stageCount = (uint32_t)shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();

        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.layout = info.builtLayout;
        pipelineInfo.renderPass = pass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

        std::vector<VkDynamicState> dynamicStates;
        dynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
        dynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);
        dynamicStates.push_back(VK_DYNAMIC_STATE_DEPTH_BIAS);
        dynamicState.pDynamicStates = dynamicStates.data();
        dynamicState.dynamicStateCount = (uint32_t)dynamicStates.size();

        pipelineInfo.pDynamicState = &dynamicState;

        VkPipeline vk_pipeline;

        if (vkCreateGraphicsPipelines(
                info.device.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vk_pipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("Unable to create graphics pipeline.");
        }
        else
        {
            // return Pipeline{vk_pipeline, info.builtLayout};
            Pipeline pipeline; // this is more verbose, I like it
            pipeline.pipeline = vk_pipeline;
            pipeline.layout = info.builtLayout;
            pipeline.bindings = bindings;
            pipeline.setLayouts = setLayouts;
            return pipeline;
        }
    };

    VkPipelineLayoutCreateInfo PipelineBuilder::populate_pipeline_layout_create_info()
    {

        VkPipelineLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        info.pNext = nullptr;

        // empty defaults
        info.flags = 0;
        info.setLayoutCount = 0;
        info.pSetLayouts = nullptr;
        info.pushConstantRangeCount = 0;
        info.pPushConstantRanges = nullptr;
        return info;
    }

    void PipelineBuilder::reflect_layout(VkDevice device, ReflectionOverrides *overrides, int overrideCount)
    {
        std::vector<DescriptorSetLayoutData> set_layouts;
        std::vector<VkPushConstantRange> constant_ranges;

        for (auto &s : stages)
        {
            SpvReflectShaderModule spvmodule;

            SpvReflectResult result = spvReflectCreateShaderModule(s.shaderModule->code.size() * sizeof(uint32_t), s.shaderModule->code.data(), &spvmodule);

            uint32_t count = 0;
            result = spvReflectEnumerateDescriptorSets(&spvmodule, &count, NULL);

            assert(result == SPV_REFLECT_RESULT_SUCCESS);

            std::vector<SpvReflectDescriptorSet *> sets(count);

            result = spvReflectEnumerateDescriptorSets(&spvmodule, &count, sets.data());

            assert(result == SPV_REFLECT_RESULT_SUCCESS);

            for (size_t i_set = 0; i_set < sets.size(); ++i_set)
            {
                const SpvReflectDescriptorSet &refl_set = *(sets[i_set]);

                DescriptorSetLayoutData layout = {};

                layout.bindings.resize(refl_set.binding_count);

                for (uint32_t i_binding = 0; i_binding < refl_set.binding_count; ++i_binding)
                {
                    const SpvReflectDescriptorBinding &refl_binding = *(refl_set.bindings[i_binding]);
                    VkDescriptorSetLayoutBinding &layout_binding = layout.bindings[i_binding];
                    layout_binding.binding = refl_binding.binding;
                    layout_binding.descriptorType = static_cast<VkDescriptorType>(refl_binding.descriptor_type);

                    spdlog::debug("Binding name: {}", refl_binding.name);

                    for (int ov = 0; ov < overrideCount; ov++)
                    {
                        if (strcmp(refl_binding.name, overrides[ov].name) == 0)
                        {
                            layout_binding.descriptorType = overrides[ov].overridenType;
                        }
                    }

                    layout_binding.descriptorCount = 1;

                    for (uint32_t i_dim = 0; i_dim < refl_binding.array.dims_count; ++i_dim)
                    {
                        layout_binding.descriptorCount *= refl_binding.array.dims[i_dim];
                    }

                    layout_binding.stageFlags = static_cast<VkShaderStageFlagBits>(spvmodule.shader_stage);

                    ReflectedBinding reflected;
                    reflected.binding = layout_binding.binding;
                    reflected.set = refl_set.set;
                    reflected.type = layout_binding.descriptorType;

                    bindings[refl_binding.name] = reflected;
                }

                layout.set_number = refl_set.set;
                layout.create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                layout.create_info.bindingCount = refl_set.binding_count;
                layout.create_info.pBindings = layout.bindings.data();

                set_layouts.push_back(layout);
            }

            result = spvReflectEnumeratePushConstantBlocks(&spvmodule, &count, NULL);
            assert(result == SPV_REFLECT_RESULT_SUCCESS);

            std::vector<SpvReflectBlockVariable *> pconstants(count);
            result = spvReflectEnumeratePushConstantBlocks(&spvmodule, &count, pconstants.data());
            assert(result == SPV_REFLECT_RESULT_SUCCESS);

            if (count > 0)
            {
                VkPushConstantRange pcs{};
                pcs.offset = pconstants[0]->offset;
                pcs.size = pconstants[0]->size;
                pcs.stageFlags = s.stage;

                constant_ranges.push_back(pcs);
            }
        }

        std::array<DescriptorSetLayoutData, 4> merged_layouts;

        for (int i = 0; i < 4; i++)
        {
            DescriptorSetLayoutData &ly = merged_layouts[i];

            ly.set_number = i;

            ly.create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

            std::unordered_map<int, VkDescriptorSetLayoutBinding> binds;

            for (auto &s : set_layouts)
            {
                if (s.set_number == i)
                {
                    for (auto &b : s.bindings)
                    {
                        auto it = binds.find(b.binding);
                        if (it == binds.end())
                        {
                            binds[b.binding] = b;
                            // ly.bindings.push_back(b);
                        }
                        else
                        {
                            // merge flags
                            binds[b.binding].stageFlags |= b.stageFlags;
                        }
                    }
                }
            }

            for (auto [k, v] : binds)
            {
                ly.bindings.push_back(v);
            }

            std::sort(ly.bindings.begin(), ly.bindings.end(), [](VkDescriptorSetLayoutBinding &a, VkDescriptorSetLayoutBinding &b)
                      { return a.binding < b.binding; });

            ly.create_info.bindingCount = (uint32_t)ly.bindings.size();
            ly.create_info.pBindings = ly.bindings.data();
            ly.create_info.flags = 0;
            ly.create_info.pNext = 0;

            if (ly.create_info.bindingCount > 0)
            {
                //     setHashes[i] = vkutil::hash_descriptor_layout_info(&ly.create_info);
                vkCreateDescriptorSetLayout(device, &ly.create_info, nullptr, &setLayouts[i]);
            }
            else
            {
                // setHashes[i] = 0;
                setLayouts[i] = VK_NULL_HANDLE;
            }

            VkPipelineLayoutCreateInfo p_layout_info = populate_pipeline_layout_create_info();

            p_layout_info.pPushConstantRanges = constant_ranges.data();

            p_layout_info.pushConstantRangeCount = (uint32_t)constant_ranges.size();

            std::array<VkDescriptorSetLayout, 4> compactedLayouts;

            int s = 0;
            for (int i = 0; i < 4; i++)
            {
                if (setLayouts[i] != VK_NULL_HANDLE)
                {
                    compactedLayouts[s] = setLayouts[i];
                    s++;
                }
            }

            p_layout_info.setLayoutCount = s;

            p_layout_info.pSetLayouts = compactedLayouts.data();

            vkCreatePipelineLayout(device, &p_layout_info, nullptr, &info.builtLayout);
        }
    }

}