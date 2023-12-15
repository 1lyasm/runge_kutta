all:
	clang -g -ansi -fsanitize=address \
	 	-Weverything src/main.c -o main
run:
	./main