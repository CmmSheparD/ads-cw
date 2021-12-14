cmake_minimum_required(VERSION 3.16)

# Adding googletest to test things. Hope it works
# I don't get how it works. Hope to get it in the future..
include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(list-test)
target_sources(list-test
	PRIVATE list-test.cpp
	PUBLIC ../src/list.hh
)

target_link_libraries(list-test gtest_main)

add_executable(parsing-table-test)
target_sources(parsing-table-test
	PRIVATE parsing-table-test.cpp
	PUBLIC ../src/parsing-table.hh
)

target_link_libraries(parsing-table-test parsing-table gtest_main)
