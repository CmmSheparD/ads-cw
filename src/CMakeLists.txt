add_library(calculation-tree STATIC)
target_sources(calculation-tree
	PRIVATE calculation-tree.cpp
	PUBLIC calculation-tree.hh
)

add_library(parsing-table STATIC)
target_sources(parsing-table
	PRIVATE parsing-table.cpp
	PUBLIC parsing-table.hh
)

add_library(parsing STATIC)
target_sources(parsing
	PRIVATE parsing.cpp
	PUBLIC parsing.hh
)

add_executable(calculator)
target_sources(calculator
	PRIVATE main.cpp
)

target_link_libraries(calculator parsing parsing-table calculation-tree)
