cmake_minimum_required(VERSION 3.5)

# The Project Name is also the Name of the resulting library
# You probably want to change this
# If you change this make sure to change the lib name in your test/CMakeLists.txt too!
project("HoneyBadger")

add_definitions(-DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING=1)
set(lib_dir "${PROJECT_SOURCE_DIR}/lib/")
file(GLOB source_files "${lib_dir}/*.cpp" "${lib_dir}/*.hpp")

include_directories("/usr/lib/llvm-6.0/include")
link_directories("/usr/lib/llvm-6.0/lib")

enable_testing()

# Here you can decide if your lib should be a shared or a static one
# https://stackoverflow.com/questions/2649334/difference-between-static-and-shared-libraries/2649430#2649430
#
add_library(${PROJECT_NAME} SHARED ${source_files})
# add_library(${PROJECT_NAME} STATIC ${source_files})

target_link_libraries(${PROJECT_NAME} "LLVM")


add_subdirectory(test)
