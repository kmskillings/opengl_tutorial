executable_name = program
objects = main.o scene.o

build_folder = ./build

include_folder = ./external/include

lib_folder = ./external/lib
libs = -lglfw3 -lGLEW -lGL -lm

src_folder = ./src
shaders_folder = ${src_folder}/shaders
textures_folder = ${src_folder}/textures

options_compile = -g -c -I${include_folder}

options_linker =
options_lib = -L${lib_folder} ${libs}

.PHONY: build
build: ${build_folder}/${executable_name}

${build_folder}/${executable_name}: ${build_folder}/main.o ${build_folder}/shaders.o
	g++ ${options_linker} -o $@ $^ ${options_lib}

${build_folder}/main.o: ${src_folder}/main.cpp
	g++ ${options_compile} -o $@ $< -I${include_folder}

${build_folder}/shaders.o: ${shaders_folder}/shaders.c ${shaders_folder}/vertex.xxd ${shaders_folder}/fragment.xxd
	gcc ${options_compile} -o $@ $<

${shaders_folder}/vertex.xxd: ${shaders_folder}/vertex.glsl
	xxd -i < $< > $@

${shaders_folder}/fragment.xxd: ${shaders_folder}/fragment.glsl
	xxd -i < $< > $@

.PHONY: clean
clean:
	rm -f ${shaders_folder}/*.xxd
	rm -f ${build_folder}/*.o
	rm -f ${build_folder}/${executable_name}