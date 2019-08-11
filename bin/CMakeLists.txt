cmake_minimum_required(VERSION 3.5)

# Name of your Library if you change this make sure to change ../CMakeLists.txt as well!
project("HoneyBadgerCompiler")
set(LIB_NAME "HoneyBadger")
set (CMAKE_CXX_STANDARD 11)

file(GLOB source_files "*.cpp")

add_executable(${PROJECT_NAME} ${source_files})

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB_NAME})
