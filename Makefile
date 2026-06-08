executable_name = program

debug_folder = ./build/debug
release_folder = ./build/release

include_folder = ./external/include

lib_folder = ./external/lib
libs = -lglfw3 -lGLEW -lGL -lm

src_folder = ./src
shaders_folder = ${src_folder}/shaders

options_compile_debug = -g -c ${options_compile_global}
options_compile_release = -c ${options_compile_global}
options_compile_global = -I${include_folder}

options_linker_debug = ${options_linker_global}
options_linker_release = ${options_linker_global}
options_linker_global =
options_lib = -L${lib_folder} ${libs}

.PHONY: debug
debug: ${debug_folder}/${executable_name}

${debug_folder}/${executable_name}: ${debug_folder}/main.o
	gcc ${options_linker_debug} -o $@ $^ ${options_lib}

${debug_folder}/main.o: ${src_folder}/main.c ${shaders_folder}/vertex.xxd ${shaders_folder}/fragment.xxd
	gcc ${options_compile_debug} -o $@ $<

${shaders_folder}/vertex.xxd: ${shaders_folder}/vertex.glsl
	xxd -i < $< > $@

${shaders_folder}/fragment.xxd: ${shaders_folder}/vertex.glsl
	xxd -i < $< > $@


.PHONY: clean
clean: clean_debug clean_release

.PHONY: clean_debug
clean_debug:
	rm -f ${debug_folder}/*.o
	rm -f ${debug_folder}/${executable_name}

.PHONY: clean_release
clean_release:
	rm -f ${release_folder}/*.o
	rm -f ${release_folder}/${executable_name}
	rm -f ${shaders_folder}/*.xxd