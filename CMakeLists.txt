cmake_minimum_required(VERSION 3.5)

project(HttpServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")
set(source_dir "${PROJECT_SOURCE_DIR}/src/")

file (GLOB source_files "${source_dir}/*.cpp")

add_executable(HttpServer ${source_files})

target_link_libraries(${PROJECT_NAME} url_decode)
target_link_libraries(${PROJECT_NAME} http_lib)
target_link_libraries(${PROJECT_NAME} file_utils)

add_subdirectory(${source_dir}/urldecode)
add_subdirectory(${source_dir}/http)
add_subdirectory(${source_dir}/utils)