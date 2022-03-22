add_definitions(-DEXAMPLES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../examples/")

add_library(contrib_catch_main ${CMAKE_SOURCE_DIR}/contrib/catch/catch_main.cpp)

target_include_directories(contrib_catch_main PUBLIC ${CMAKE_SOURCE_DIR}/contrib/catch)

add_executable(test_image_io test_image_io.cpp)
target_link_libraries(test_image_io PRIVATE lib contrib_catch_main)
add_test(test test_image_io)
