# https://github.com/google/googletest/tree/master/googletest


# Download and unpack googletest at configure time

configure_file(download.cmake googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
		RESULT_VARIABLE result
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
		RESULT_VARIABLE result
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")




# Add googletest directly to our build. This defines the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src ${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
	include_directories(BEFORE SYSTEM
			"${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")
else()
	target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
			"${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")
endif()



