set(FORMATXX_TEST OFF CACHE BOOL "enable formatxx tests")
add_subdirectory(formatxx EXCLUDE_FROM_ALL)

if(LOGXX_BUILD_TESTS)
    set(DOCTEST_WITH_TESTS OFF CACHE BOOL "enable doctest tests")
    set(DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF CACHE BOOL "enable doctest static lib")
    add_subdirectory(doctest EXCLUDE_FROM_ALL)
endif()
