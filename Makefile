example := textures
output_dir := bin
output := $(output_dir)/$(example)

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
CFLAGS = -Iinclude -lglfw -ldl -framework OpenGL
endif

ifeq ($(UNAME), Linux)
CFLAGS = -Iinclude -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
endif

CFILES = $(example).cpp glad.c shader.cpp stb_image.cpp

build:
	mkdir -p $(output_dir)
	clang++ $(CFLAGS) $(CFILES) -o $(output)

run: build
	./$(output)
