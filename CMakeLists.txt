cmake_minimum_required(VERSION 3.15)
project(TaskReplacer C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

option(BUILD_SHARED_LIBS "Build libraries as shared(dynamic)" OFF)
option(BUILD_TESTING "Build the testing suite" ON)

#для cmake -B build -DN=2048
set(N 1024 CACHE STRING "Size of the read buffer N")
add_compile_definitions(N=${N})

if(MSVC)
    add_compile_options(/wd5045)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    add_link_options("msvcrt.lib" "legacy_stdio_definitions.lib")
endif()

if(BUILD_TESTING)
    enable_testing()
    
    add_custom_target(test_all
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMENT "Running tests"
    )
endif()

add_subdirectory(lib)
add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
