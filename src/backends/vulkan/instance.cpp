#include "instance.hpp"

#include "extensions.hpp"
#include "vk_util.hpp"

#include <spdlog/spdlog.h>

namespace rush
{
    //building doesnt do anything :))
//     InstanceBuilder::InstanceBuilder() {}
    
//     Instance &InstanceBuilder::build() const
//     {
//         // get available extensions and layers
//         auto sys_inf = rush::get_system_info();
//         // init app info struct
        
//         VkApplicationInfo app_info = {};
//         app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//         app_info.pNext = nullptr;
//         app_info.pApplicationName = info.application_name != nullptr ? info.application_name : "";
//         app_info.applicationVersion = info.application_v;
//         app_info.pEngineName = info.engine_name != nullptr ? info.engine_name : "";
//         app_info.engineVersion = info.engine_v;
//         app_info.apiVersion = VK_API_VERSION_1_0;

//         // init layers for future reference
//         std::vector<const char *> layers;

//         // GET REQUIRED EXTENSIONS
//         uint32_t glfwExtensionCount = 0;
//         const char **glfwExtensions;
//         glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//         std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

//         for (auto &ext : info.extensions)
//             extensions.push_back(ext);

//         if (info.debug_callback != nullptr && sys_inf.debug_utils_available)
//         {
//             extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//         }

//         // macOS fuckery -- always push back even if not necessary, can provide some xtra features in the future
//         bool supports_properties2_ext =
//             rush::check_extension_supported(sys_inf.extensions, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

//         if (supports_properties2_ext)
//         {
//             extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
//         }

// #if defined(VK_KHR_portability_enumeration)
//         bool portability_enumeration_support =
//             rush::check_extension_supported(sys_inf.extensions, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
//         if (portability_enumeration_support)
//         {
//             extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
//         }
// #else
//         bool portability_enumeration_support = false;
// #endif

//         // ensure every extension on the stack (user pushed or required) is supported
//         bool all_extensions_supported = rush::check_extensions_supported(sys_inf.extensions, extensions);
//         if (!all_extensions_supported)
//         {
//             // spdlog::critical("Requested extension not present");
//             throw std::runtime_error("Requested vulkan extension not present");
//         }


//         for (auto &layer : info.layers)
//             layers.push_back(layer);

//         // enable validation layers
//         if (info.enable_validation_layers || (info.request_validation_layers && sys_inf.validation_layers_available))
//         {
//             layers.push_back("VK_LAYER_KHRONOS_validation");
//         }


//         std::vector<VkBaseOutStructure *> pNext_chain;

//         VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = {};
//         if (info.use_debug_messenger)
//         {
//             messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
//             messengerCreateInfo.pNext = nullptr;
//             messengerCreateInfo.messageSeverity = info.debug_message_severity;
//             messengerCreateInfo.messageType = info.debug_message_type;
//             messengerCreateInfo.pfnUserCallback = info.debug_callback;
//             messengerCreateInfo.pUserData = info.debug_user_data_pointer;
//             pNext_chain.push_back(reinterpret_cast<VkBaseOutStructure *>(&messengerCreateInfo));
//         }

//         VkInstanceCreateInfo create_info = {};
//         create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//         rush::setup_pNext_chain(create_info, pNext_chain);

//         create_info.flags = info.flags;
//         create_info.pApplicationInfo = &app_info;
//         create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
//         create_info.ppEnabledExtensionNames = extensions.data();
//         create_info.enabledLayerCount = static_cast<uint32_t>(layers.size());
//         create_info.ppEnabledLayerNames = layers.data();

// #if defined(VK_KHR_portability_enumeration)

//         if (portability_enumeration_support)
//         {
//             create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
//         }

// #endif
//         Instance vk_instance;

//         if (vkCreateInstance(&create_info, info.allocation_callbacks, &vk_instance.instance) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to create vkInstance!");
//         }

//         if (info.use_debug_messenger)
//         {
//             VkResult msnger = create_debug_utils_messenger(
//                 vk_instance.instance, 
//                 info.debug_callback, 
//                 info.debug_message_severity, 
//                 info.debug_message_type, 
//                 info.debug_user_data_pointer, 
//                 &vk_instance.debug_messenger, 
//                 info.allocation_callbacks);

//                 if(msnger !=VK_SUCCESS){
//                     throw std::runtime_error("Failed to create debug messenger");
//                 }
//         }

//         vk_instance.api_version = info.api_v;
//         vk_instance.supports_properties2_ext = supports_properties2_ext;
//         //to change. get required VK versions, not just 1.0
//         vk_instance.instance_version = info.api_v;

//         return vk_instance;
//     }

//     InstanceBuilder &InstanceBuilder::set_app_name(const char *app_name)
//     {
//         if (!app_name)
//             return *this;
//         info.application_name = app_name;
//         return *this;
//     };

//     InstanceBuilder &InstanceBuilder::set_engine_name(const char *engine_name)
//     {
//         if (!engine_name)
//             return *this;
//         info.engine_name = engine_name;
//         return *this;
//     };

//     InstanceBuilder &InstanceBuilder::set_app_version(uint32_t major, uint32_t minor, uint32_t patch)
//     {
//         info.application_v = VK_MAKE_VERSION(major, minor, patch);
//         return *this;
//     }

//     InstanceBuilder &InstanceBuilder::set_engine_version(uint32_t major, uint32_t minor, uint32_t patch)
//     {
//         info.engine_v = VK_MAKE_VERSION(major, minor, patch);
//         return *this;
//     }

//     InstanceBuilder &InstanceBuilder::enable_layer(const char *layer)
//     {
//         if (!layer)
//             return *this;
//         info.layers.push_back(layer);
//         return *this;
//     }

//     InstanceBuilder &InstanceBuilder::enable_extension(const char *extension_name)
//     {
//         if (!extension_name)
//             return *this;
//         info.extensions.push_back(extension_name);
//         return *this;
//     }

//     InstanceBuilder &InstanceBuilder::request_validation_layers(bool enable_validation)
//     {
//         info.request_validation_layers = enable_validation;
//         return *this;
//     }

//     InstanceBuilder &InstanceBuilder::enable_validation_layers(bool enable_validation)
//     {
//         info.enable_validation_layers = enable_validation;
//         return *this;
//     }

//     InstanceBuilder &InstanceBuilder::set_debug_callback(PFN_vkDebugUtilsMessengerCallbackEXT callback)
//     {
//         info.use_debug_messenger = true;
//         info.debug_callback = callback;
//         return *this;
//     }

//     InstanceBuilder &InstanceBuilder::set_allocation_callbacks(VkAllocationCallbacks *callbacks)
//     {
//         info.allocation_callbacks = callbacks;
//         return *this;
//     }

}