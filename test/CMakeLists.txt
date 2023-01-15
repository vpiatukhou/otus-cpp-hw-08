set(BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

configure_file(Resources.h.in ${CMAKE_CURRENT_BINARY_DIR}/generated_sources/Resources.h @ONLY)

add_executable(bayan_test ConversionUtilTest.cpp DirectoryScannerTest.cpp FileComparerTest.cpp StringMatcherTest.cpp)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

target_include_directories(bayan_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/generated_sources)
target_link_libraries(bayan_test bayan GTest::gtest GTest::gtest_main GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(bayan_test)

enable_testing()

file(COPY "test-data" DESTINATION .)
