add_library(lib STATIC)

target_sources( lib
    PRIVATE
        impl/args_handler.cpp
        impl/filters.cpp
        impl/image.cpp)

target_include_directories( lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/impl)
