add_library(lib STATIC)

target_sources(lib
        PRIVATE
        impl/args_handler.cpp
        impl/bmp_reader.cpp
        impl/bmp_writer.cpp
        impl/crop.cpp
        impl/crystallize.cpp
        impl/edge_detection.cpp
        impl/filter.cpp
        impl/gauss_blur.cpp
        impl/grayscale.cpp
        impl/image.cpp
        impl/negative.cpp
        impl/sharpening.cpp)

target_include_directories(lib
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/impl)
