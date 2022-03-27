add_definitions(-DEXAMPLE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../examples/example.bmp")

add_library(catch ${CMAKE_SOURCE_DIR}/contrib/catch/catch_main.cpp)

target_include_directories(catch PUBLIC ${CMAKE_SOURCE_DIR}/contrib/catch)

add_executable(test_image_io test_image_io.cpp)
target_link_libraries(test_image_io PRIVATE lib catch)
add_test(test_image_io test_image_io)

add_executable(test_args_handler test_args_handler.cpp)
target_link_libraries(test_args_handler PRIVATE lib catch)
add_test(test_args_handler test_args_handler)

add_executable(test_filters test_filters.cpp)
target_link_libraries(test_filters PRIVATE lib catch)
add_test(test_filters test_filters)
