#include "shader.hpp"

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

    ShaderStage ShaderBuilder::build()
    {
        ShaderStage shaderStage;

        const auto shader_code = read_file(info.filepath, info.stage);

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
    // TODO: MAKE THIS A LOT BETTER WITH CACHING AND SHIT
    const std::vector<uint32_t> ShaderBuilder::read_file(const std::string &filepath, VkShaderStageFlagBits stage)
    {
        std::filesystem::path path = std::filesystem::path{ENGINE_DIR}.append(filepath);
        const std::string file_extension = path.extension().string();

        std::ifstream file{path.string(), std::ios::ate | std::ios::binary};

        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file: " + path.string());
        }

        size_t fileSize = static_cast<size_t>(file.tellg());

        if ((file_extension.find("spv") != std::string::npos) || (file_extension.find("spvasm") != std::string::npos))
        {
            spdlog::debug("using precompiled binary shader");
            std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
            file.seekg(0);
            file.read((char *)buffer.data(), fileSize); // read as char* since that is what the file is. Will read the uint32_t into the buffer instead. cred vkblanco
            file.close();
            return buffer;
        }
        else
        {
            spdlog::debug("compiling file on our own");
            shaderc::Compiler compiler;
            shaderc::CompileOptions options;

            if (true)
                options.SetOptimizationLevel(shaderc_optimization_level_size); // hardcoded for now

            shaderc_shader_kind kind;

            if (stage == VK_SHADER_STAGE_VERTEX_BIT)
            {
                kind = shaderc_vertex_shader;
            }
            else if (stage == VK_SHADER_STAGE_FRAGMENT_BIT)
            {
                kind = shaderc_fragment_shader;
            }

            std::vector<char> buffer(fileSize);
            file.seekg(0);
            file.read(buffer.data(), fileSize); // read as char* since that is what the file is. Will read the uint32_t into the buffer instead. cred vkblanco
            file.close();

            shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(std::string(buffer.begin(), buffer.end()), kind, path.filename().string().c_str(), options);

            if (module.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                spdlog::error("Module failed to compile : {}", module.GetErrorMessage());
                throw std::runtime_error("Module failed to compile : " + module.GetErrorMessage());
            }

            return {module.cbegin(), module.cend()};
// #endif
        }
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