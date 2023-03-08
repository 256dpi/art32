if(DEFINED IDF_PATH)
    file(GLOB srcs ./src/*.c)

    idf_component_register(SRCS ${srcs}
        INCLUDE_DIRS "include"
    )
else()
    cmake_minimum_required(VERSION 3.14)
    project(art32)

    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 14)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter")

    include(FetchContent)
    FetchContent_Declare(
            googletest # 1.13.0
            URL https://github.com/google/googletest/archive/b796f7d44681514f58a683a3a71ff17c94edb0c1.zip
            DOWNLOAD_EXTRACT_TIMESTAMP true
    )
    FetchContent_MakeAvailable(googletest)

    include_directories(include)

    set(SOURCE_FILES
            src/filter.c
            src/matrix.c
            src/motion.c
            src/numbers.c
            src/parser.c
            src/smooth.c
            src/strconv.c
            src/vector.c)

    add_library(art32 ${SOURCE_FILES})

    add_executable(example main.c)
    target_link_libraries(example art32)

    set(TEST_FILES
            tests/numbers.cpp
            tests/strconv.cpp)

    add_executable(tests ${TEST_FILES})
    target_link_libraries(tests art32 gtest gtest_main)

endif()
