cmake_minimum_required(VERSION 3.12)
project(cpputil)

include(GNUInstallDirs)

add_library(nostdx INTERFACE)
add_library(em::nostdx ALIAS nostdx)

option(EM_NOSTDX_BUILD_TESTS "Build the nostdx tests" OFF)

target_include_directories(nostdx INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

install(TARGETS nostdx EXPORT nostdxTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT nostdxTargets FILE nostdxTargets.cmake
        NAMESPACE em
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nostdx)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/nostdx/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nostdx)

if (EM_NOSTDX_BUILD_TESTS)
    add_executable(nostdx_tests)
    set_property(TARGET nostdx_tests PROPERTY CXX_STANDARD 17)
    if (MSVC)
        target_compile_options(nostdx_tests PRIVATE /W4 /permissive-)
    else ()
        target_compile_options(nostdx_tests PRIVATE -Wall -Wextra -Wshadow -Wunused -Wpedantic)
    endif ()

    target_sources(nostdx_tests PRIVATE
            include/nostdx/propagate_const.hpp
            include/nostdx/type_traits.hpp
            test/propagate_const.cpp)
    target_link_libraries(nostdx_tests PRIVATE em::nostdx)
endif ()
