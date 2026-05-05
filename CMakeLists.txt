cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(GlobedCompanion VERSION 1.0.0)

# Accept GEODE_SDK as either a cmake variable or environment variable
if (NOT DEFINED GEODE_SDK)
    if (DEFINED ENV{GEODE_SDK})
        set(GEODE_SDK $ENV{GEODE_SDK})
    else()
        message(FATAL_ERROR "GEODE_SDK is not set. Pass -DGEODE_SDK=<path> or set the GEODE_SDK environment variable.")
    endif()
endif()

# Fixes fmt consteval build error with Android NDK clang
set(FMT_USE_CONSTEVAL 0)
add_compile_definitions(FMT_USE_CONSTEVAL=0)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(${PROJECT_NAME} SHARED ${SOURCES})

add_subdirectory(${GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
target_link_libraries(${PROJECT_NAME} geode-sdk)
create_geode_file(${PROJECT_NAME})
