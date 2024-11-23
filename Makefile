UNAME := $(shell uname)
EXAMPLE = textures
OUTPUT_DIR = bin
OUTPUT = $(OUTPUT_DIR)/$(EXAMPLE)

CFILES = $(EXAMPLE).cpp \
	glad.c \
	shader.cpp \
	stb_image.cpp

CFLAGS = -Iinclude -lglfw -ldl -lX11 -lpthread

ifeq ($(UNAME), Darwin)
	CFLAGS += -framework OpenGL
else
	CFLAGS += -lGL -lXrandr -lXi
endif


build:
	mkdir -p $(OUTPUT_DIR)
	clang++ $(CFLAGS) $(CFILES) -o $(OUTPUT)

run: build
	./$(OUTPUT)
