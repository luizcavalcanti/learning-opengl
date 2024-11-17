build:
	clang++ hello.cpp glad.c -o hello -Iinclude -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl 

run: build
	./hello
