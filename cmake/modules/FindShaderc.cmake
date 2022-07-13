# Taken from https://github.com/thegeeko/geg/blob/master/geg/cmake/modules/FindShaderc.cmake#L3. 

include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

find_library(SHADERC_LIBRARY_DEBUG NAMES shaderc_sharedd HINTS $ENV{VULKAN_SDK} PATH_SUFFIXES lib Debug/lib)
find_library(SHADERC_LIBRARY_RELEASE NAMES shaderc_shared HINTS $ENV{VULKAN_SDK} PATH_SUFFIXES lib)
find_path(SHADERC_INLCUDE_DIRS NAMES shaderc/shaderc.h HINTS $ENV{VULKAN_SDK} PATH_SUFFIXES include)
find_program(GLSLANG_VALIDATOR_EXE NAMES glslangValidator HINTS $ENV{VULKAN_SDK} PATH_SUFFIXES bin)

select_library_configurations(SHADERC)

find_package_handle_standard_args(
	Shaderc
	DEFAULT_MSG
	SHADERC_LIBRARY
	SHADERC_INLCUDE_DIRS
	GLSLANG_VALIDATOR_EXE
)

message(STATUS "${SHADERC_LIBRARY_DEBUG}")

if((${SHADERC_LIBRARY_DEBUG} MATCHES "shaderc_sharedd") AND (${CMAKE_BUILD_TYPE} STREQUAL "Release"))
	message(FATAL_ERROR "Shaderc and ${PROJECT_NAME} build type mismatch. Please build shaderc in Release mode.")
endif()

if((NOT ${SHADERC_LIBRARY_DEBUG} MATCHES "shaderc_sharedd") AND (${CMAKE_BUILD_TYPE} STREQUAL "Debug"))
	message(FATAL_ERROR "Shaderc and ${PROJECT_NAME} build type mismatch. Please install or compile the shaderc Debug mode.")
endif()

add_executable(glslangValidator IMPORTED)
set_target_properties(glslangValidator PROPERTIES IMPORTED_LOCATION ${GLSLANG_VALIDATOR_EXE})

add_library(shaderc UNKNOWN IMPORTED)

set_target_properties(
	shaderc
		PROPERTIES
			IMPORTED_LOCATION_DEBUG ${SHADERC_LIBRARY_DEBUG}
			IMPORTED_LOCATION_RELEASE ${SHADERC_LIBRARY_RELEASE}
			IMPORTED_LOCATION ${SHADERC_LIBRARY_RELEASE}
			INTERFACE_INCLUDE_DIRECTORIES ${SHADERC_INLCUDE_DIRS}
)