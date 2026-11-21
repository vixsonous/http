add_library(file_utils SHARED file.cpp)

target_include_directories(file_utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})