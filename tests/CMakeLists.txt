add_executable(run_tests test_replacer.c)

target_link_libraries(run_tests PRIVATE replacer_lib)

add_test(NAME ReplacerTestSuite COMMAND run_tests)