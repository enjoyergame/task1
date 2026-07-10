add_library(replacer_lib
    src/replacer.c
    src/utils.c
    src/utils.h
    include/replacer.h
)

target_include_directories(replacer_lib
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
