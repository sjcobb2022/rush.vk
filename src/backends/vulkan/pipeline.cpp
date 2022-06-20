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
        return Pipeline{};
    };


}