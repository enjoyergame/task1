cmake_minimum_required(VERSION 3.15)
project(task1)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(tests)

if(MSVC)
    add_compile_options(/wd5045)
    
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
	link_libraries(msvcrt.lib)
	link_libraries(legacy_stdio_definitions.lib)
	
endif()

option(BUILD_SHARED_LIBS "Build libraries as shared (dynamic)" OFF)
option(BUILD_TESTING "Build the testing suite" ON)

if(BUILD_TESTING)
    enable_testing()
endif()

if(BUILD_TESTING)
    add_custom_target(test_all
        COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --output-on-failure
        COMMENT "Running ALL tests..."
    )
endif()

add_compile_definitions(N=1024)