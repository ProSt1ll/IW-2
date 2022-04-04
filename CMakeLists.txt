cmake_minimum_required(VERSION 3.20)
project(IW_2 C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic  --coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wpedantic --coverage")

set(SOURCES_FOR_STATIC libs/recommendation/src/recommendation.c)
set(SOURCES_FOR_SHARED libs/recommendation_forked/src/recommendation_forked.c)

add_library(recommendation_forked SHARED ${SOURCES_FOR_SHARED})
target_include_directories(recommendation_forked PUBLIC libs/recommendation_forked/include)

add_library(recommendation STATIC ${SOURCES_FOR_STATIC})
target_include_directories(recommendation PUBLIC libs/recommendation/include)

add_executable(main_prog main/main.c)
target_link_libraries(main_prog recommendation recommendation_forked)
find_package(GTest REQUIRED)

add_executable(tests tests/tests.cpp)
target_link_libraries(tests GTest::gtest recommendation)

add_executable(tests_forked tests/tests_forked.cpp)
target_link_libraries(tests_forked GTest::gtest recommendation_forked)

add_executable(stress_test tests/stress_test.cpp)
target_link_libraries(stress_test GTest::gtest recommendation)

add_executable(stress_test_forked tests/stress_test_forked.cpp)
target_link_libraries(stress_test_forked GTest::gtest recommendation_forked)