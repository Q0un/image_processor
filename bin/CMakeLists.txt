add_executable(image_processor)
target_sources(image_processor
    PRIVATE
        main.cpp
)
target_link_libraries(image_processor PRIVATE lib)