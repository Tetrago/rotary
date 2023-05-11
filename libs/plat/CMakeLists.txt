cmake_minimum_required(VERSION 3.8)
project(plat LANGUAGES CXX)

function(plat_module)
	cmake_parse_arguments(
		ARGV
		""
		"NAME"
		"LIBRARIES;DEFINITIONS"
		${ARGN}
	)

	string(TOUPPER ${ARGV_NAME} OPT)

	option(${OPT} "" ON)
	if(${OPT})
		set(LIB plat-${ARGV_NAME})

		add_library(${LIB} STATIC "include/plat/${ARGV_NAME}.hpp" "src/${ARGV_NAME}.cpp")

		target_compile_features(${LIB} PUBLIC cxx_std_20)
		target_include_directories(${LIB} PUBLIC "include")

		if(DEFINED ARGV_LIBRARIES)
			target_link_libraries(${LIB} ${ARGV_LIBRARIES})
		endif()

		if(DEFINED ARGV_DEFINITIONS)
			target_compile_definitions(${LIB} ${ARGV_DEFINITIONS})
		endif()
	endif()
endfunction()

plat_module(NAME window LIBRARIES PRIVATE glfw)
plat_module(NAME vulkan LIBRARIES PUBLIC Vulkan::Headers PRIVATE plat-window glfw)
plat_module(NAME filesystem)
