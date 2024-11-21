example := shaders
output_dir := bin
output := $(output_dir)/$(example)

build:
	mkdir -p $(output_dir)
	clang++ -o $(output) -Iinclude -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl $(example).cpp glad.c shader.cpp

run: build
	./$(output)
