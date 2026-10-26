cmake_minimum_required(VERSION 3.16)
project(oop_lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_SANITIZERS "Enable AddressSanitizer and UBSan" OFF)

if(ENABLE_SANITIZERS)
    if(MSVC)
        message(WARNING "Sanitizers are not supported on MSVC, using /RTC instead")
        add_compile_options(/RTC1)
    else()
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

add_library(oop_lab2_lib src/polynomial.cpp)
target_include_directories(oop_lab2_lib PUBLIC src)

add_executable(oop_lab2_main main.cpp)
target_link_libraries(oop_lab2_main oop_lab2_lib)

if(MSVC)
    target_compile_options(oop_lab2_main PRIVATE /W4 /WX)
else()
    target_compile_options(oop_lab2_main PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(FetchContent)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(oop_lab2_tests tests/test.cpp)
target_link_libraries(oop_lab2_tests oop_lab2_lib GTest::gtest_main)
if(MSVC)
    target_compile_options(oop_lab2_tests PRIVATE /W4 /WX)
else()
    target_compile_options(oop_lab2_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()
include(GoogleTest)
gtest_discover_tests(oop_lab2_tests)