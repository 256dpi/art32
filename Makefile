fmt:
	clang-format -i ./src/*.c ./include/art32/*.h ./tests/*.cpp -style="{BasedOnStyle: Google, ColumnLimit: 120, SortIncludes: false}"
