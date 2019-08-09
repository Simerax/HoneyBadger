cmake_minimum_required(VERSION 3.5)

# The Project Name is also the Name of the resulting library
# You probably want to change this
# If you change this make sure to change the lib name in your test/CMakeLists.txt too!
project("HoneyBadger")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
set(lib_dir "${PROJECT_SOURCE_DIR}/lib/")
file(GLOB source_files "${lib_dir}/*.cpp" "${lib_dir}/*.hpp")

set(LLVM_ROOT "${PROJECT_SOURCE_DIR}/ext/llvm")
if( NOT EXISTS ${LLVM_ROOT}/include/llvm)
    message(FATAL_ERROR "LLVM_ROOT (${LLVM_ROOT}) is not valid")
endif()

add_definitions(-DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING=1)
include_directories("${LLVM_ROOT}/include/")
include_directories("${LLVM_ROOT}/build/include/")
link_directories("${LLVM_ROOT}/lib/")

enable_testing()

# Here you can decide if your lib should be a shared or a static one
# https://stackoverflow.com/questions/2649334/difference-between-static-and-shared-libraries/2649430#2649430
#
add_library(${PROJECT_NAME} SHARED ${source_files})
# add_library(${PROJECT_NAME} STATIC ${source_files})



add_subdirectory(test)
