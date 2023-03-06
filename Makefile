test:
	gcc -Iinclude src/*.c main.c -lm
	./a.out
	rm a.out

fmt:
	clang-format -i ./main.c ./src/*.c ./include/art32/*.h -style="{BasedOnStyle: Google, ColumnLimit: 120, SortIncludes: false}"
