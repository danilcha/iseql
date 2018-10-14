cmake_minimum_required(VERSION 2.8.2)

# https://github.com/google/googletest/tree/master/googletest

project(googletest-download NONE)

include(ExternalProject)

ExternalProject_Add(googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG release-1.8.0
#		GIT_CONFIG advice.detachedHead=false
		SOURCE_DIR "${CMAKE_BINARY_DIR}/googletest-src"
		BINARY_DIR "${CMAKE_BINARY_DIR}/googletest-build"
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
		TEST_COMMAND ""
		UPDATE_COMMAND ""
)
