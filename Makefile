fmt:
	clang-format -i ./main.c ./src/*.c ./include/art32/*.h -style="{BasedOnStyle: Google, ColumnLimit: 120, SortIncludes: false}"
