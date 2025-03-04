# tests/CMakeLists.txt
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)

add_executable(point_tests point_tests.cpp)
target_link_libraries(point_tests
    PRIVATE
    femto
    GTest::gtest_main
    Threads::Threads
)

add_executable(thread_tests thread_tests.cpp)
target_link_libraries(thread_tests
    PRIVATE
    femto
    GTest::gtest_main
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(point_tests)
gtest_discover_tests(thread_tests)
