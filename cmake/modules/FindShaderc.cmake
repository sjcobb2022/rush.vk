# Taken from https://github.com/thegeeko/geg/blob/master/geg/cmake/modules/FindShaderc.cmake#L3. 

include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

if(WIN32)
	set(SHADERC_ENABLE_SHARED_CRT ON CACHE BOOL "" FORCE)
endif()

find_library(SHADERC_LIBRARY_DEBUG NAMES shaderc_combinedd HINTS $ENV{VULKAN_SDK} PATH_SUFFIXES DEBUG/lib DEBUG/Lib)
find_library(SHADERC_LIBRARY_RELEASE NAMES shaderc_combined HINTS $ENV{VULKAN_SDK} PATH_SUFFIXES lib)
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

add_executable(glslangValidator IMPORTED)
set_target_properties(glslangValidator PROPERTIES IMPORTED_LOCATION ${GLSLANG_VALIDATOR_EXE})

add_library(shaderc UNKNOWN IMPORTED)

set_target_properties(
	shaderc
		PROPERTIES
			IMPORTED_LOCATION ${SHADERC_LIBRARY}
			INTERFACE_INCLUDE_DIRECTORIES ${SHADERC_INLCUDE_DIRS}
)