# Taken from https://github.com/thegeeko/geg/blob/master/geg/cmake/modules/FindShaderc.cmake#L3. 

if(DEFINED ENV{VULKAN_SDK})
	# Use ShaderC from VulkanSDK
	if(WIN32)
		if (CMAKE_BUILD_TYPE MATCHES "Debug")
			set(SHADERC_LIB "$ENV{VULKAN_SDK}/Lib/shaderc_combinedd.lib")
		else()
			set(SHADERC_LIB "$ENV{VULKAN_SDK}/Lib/shaderc_combined.lib")
		endif()
	else()
		# on unix based systems
		set(SHADERC_LIB "$ENV{VULKAN_SDK}/lib/libshaderc_combined.a")
		set(STATUS "sdfsdf")
	endif()
else()	
	message(STATUS "VulkanSDK not found looking local install")
	if(WIN32)
		message(FATAL_ERROR "You have to install VulkanSDK and add the env VULKAN_SDK")
	else()
		find_library(SHADERC_LIB shaderc_combined REQUIRED)
	endif()
endif()

message(STATUS "found shaderc in the path ${SHADERC_LIB}")

add_library(shaderc STATIC IMPORTED)
set_property(TARGET shaderc PROPERTY IMPORTED_LOCATION ${SHADERC_LIB})