cmake_minimum_required(VERSION 3.7)
project(art32)

set(CMAKE_C_STANDARD 99)

include_directories(
        test/build/include
        test/xtensa-esp32-elf/xtensa-esp32-elf/include
        test/esp-idf/components/esp32/include
        test/esp-idf/components/freertos/include
        test/esp-idf/components/log/include
        include/)

set(SOURCE_FILES
        include/art32/matrix.h
        include/art32/motion.h
        include/art32/numbers.h
        include/art32/smooth.h
        include/art32/strconv.h
        include/art32/vector.h
        src/matrix.c
        src/motion.c
        src/numbers.c
        src/smooth.c
        src/strconv.c
        src/vector.c
        test/main/main.c)

add_library(art32 ${SOURCE_FILES})
