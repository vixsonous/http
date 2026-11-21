add_library(http_lib SHARED http.cpp)
target_include_directories(http_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(http_lib PUBLIC url_decode)
target_link_libraries(http_lib PUBLIC file_utils)