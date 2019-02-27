cmake_minimum_required(VERSION 3.12)

project(logxx
	VERSION 0.0.1
	HOMEPAGE_URL https://github.com/seanmiddleditch/logxx.git
	LANGUAGES CXX
)

option(LOGXX_BUILD_TESTS "Build logxx tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_library(logxx)
target_compile_features(logxx PUBLIC cxx_std_11)
set_target_properties(logxx PROPERTIES
    DEFINE_SYMBOL LOGXX_EXPORT
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

target_sources(logxx PRIVATE
	include/logxx/logxx.h
    include/logxx/logger_ostream.h
    include/logxx/logger_stdio.h
)
target_sources(logxx PRIVATE
	source/logxx.cpp
    source/logger_ostream.cpp
    source/logger_stdio.cpp
)

target_include_directories(logxx PUBLIC
	$<INSTALL_INTERFACE:include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(TARGETS logxx EXPORT logxx-targets
    ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_PREFIX})
install(EXPORT logxx-targets DESTINATION share/logxx/cmake)

export(TARGETS logxx FILE logxx-exports.cmake)

if(LOGXX_BUILD_TESTS)
	enable_testing()
	add_executable(logxx_tests)
    target_sources(logxx_tests PRIVATE
        tests/main.cpp
        tests/test_ostream.cpp
        tests/test_stdio.cpp
    )
	target_link_libraries(logxx_tests logxx)
	add_test(logxx_tests logxx_tests)
endif()
