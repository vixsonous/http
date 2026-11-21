add_library(url_decode SHARED urldecode.cpp)
target_include_directories(url_decode INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})