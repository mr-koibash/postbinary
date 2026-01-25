cmake_minimum_required(VERSION 3.5.0)
project(test1 VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(
    main

    main.cpp
    src/abstracts/Fractional.cpp
    src/abstracts/Number.cpp
    src/utilities/ByteOrder.cpp
    src/utilities/Convert.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
