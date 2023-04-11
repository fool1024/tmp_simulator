cmake_minimum_required (VERSION 3.3.2)

project (acc_sim)

set (CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O0 -Wall")

include_directories (
    inc
    ../3rd/googletest/googletest/include
)

link_directories (
    ../3rd/googletest/lib
)

aux_source_directory (src SRCS)

add_library (all_src STATIC ${SRCS})

aux_source_directory (test TESTS)

foreach (v ${TESTS})
    string (REGEX MATCH "test/.*" relative_path ${v})
    string (REGEX REPLACE "test/" "" target_name ${relative_path})
    string (REGEX REPLACE ".cpp" "" target_name ${target_name})
    add_executable (test_${target_name} test/${target_name}.cpp)
    target_link_libraries (test_${target_name} gtest pthread all_src)
endforeach()

add_custom_target(cpsh
    ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/*.sh ${PROJECT_BINARY_DIR}
)