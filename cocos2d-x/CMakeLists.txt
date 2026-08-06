cmake_minimum_required(VERSION 3.24)
project(unreal_unity_cocos2dx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(rust_earth_controller STATIC
    src/RustEarthController.cpp
)
target_include_directories(rust_earth_controller PUBLIC include src)
target_compile_options(rust_earth_controller PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic;-Werror>
)

include(CTest)
if(BUILD_TESTING)
    add_executable(rust_earth_controller_test
        tests/controller_test.cpp
        tests/fake_rust_engine.cpp
    )
    target_link_libraries(rust_earth_controller_test PRIVATE rust_earth_controller)
    target_compile_options(rust_earth_controller_test PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic;-Werror>
    )
    add_test(NAME rust_earth_controller_test COMMAND rust_earth_controller_test)
endif()
