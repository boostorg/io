# Copyright 2018 Mike Dev
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt
#
# NOTE: This does NOT run the unit tests for Boost.IO.
#       It only tests, if the CMakeLists.txt file in the
#       Boost.IO root directory works as expected.

cmake_minimum_required( VERSION 3.5 )

project( BoostIoCMakeSelfTest )

add_subdirectory( ../../../config ${CMAKE_CURRENT_BINARY_DIR}/libs/config )
add_subdirectory( ../.. ${CMAKE_CURRENT_BINARY_DIR}/libs/io )

add_executable( boost_io_cmake_self_test main.cpp )
target_link_libraries( boost_io_cmake_self_test Boost::io )
