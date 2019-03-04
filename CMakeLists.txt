cmake_minimum_required(VERSION 3.12)

project(logxx
	VERSION 0.0.1
	HOMEPAGE_URL https://github.com/seanmiddleditch/logxx.git
	LANGUAGES CXX
)

option(LOGXX_BUILD_TESTS "Build logxx tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(LOGXX_RELEASE_SOURCE_LOCATION "Always output source locations" OFF)

add_subdirectory(external)

add_library(logxx)
target_compile_features(logxx PUBLIC cxx_std_11)
target_link_libraries(logxx PUBLIC formatxx)
set_target_properties(logxx PROPERTIES
    DEFINE_SYMBOL LOGXX_EXPORT
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

if(LOGXX_RELEASE_SOURCE_LOCATION)
    target_compile_definitions(logxx PUBLIC
        LOGXX_USE_SOURCE_LOCATION=1
    )
endif()

target_sources(logxx PRIVATE
	include/logxx/logxx.h
    include/logxx/log_format.h
    include/logxx/logger_null.h
    include/logxx/logger_ostream.h
    include/logxx/logger_outputdebugstring.h
    include/logxx/logger_splitter.h
    include/logxx/logger_stdio.h
)
target_sources(logxx PRIVATE
    source/log_format.cpp
	source/logxx.cpp
    source/logger_ostream.cpp
    $<$<PLATFORM_ID:Windows>:source/logger_outputdebugstring.cpp>
    source/logger_stdio.cpp
    source/reader_writer_lock.h
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
        tests/test_log_format.cpp
        tests/test_ostream.cpp
        $<$<PLATFORM_ID:Windows>:tests/test_outputdebugstring.cpp>
        tests/test_splitter.cpp
        tests/test_stdio.cpp
    )
	target_link_libraries(logxx_tests logxx doctest)
	add_test(logxx_tests logxx_tests)
endif()
