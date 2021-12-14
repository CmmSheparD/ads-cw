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
