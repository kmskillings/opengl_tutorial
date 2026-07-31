dir_build := ./build
program_name := program

src_dir := ./src

src_cpp_names := \
camiCubeSystem.cpp \
camiCubeVao.cpp \
chunkingSystem.cpp \
collisionBroadSystem.cpp \
meshSphere.cpp \
inputSystem.cpp \
playerControlSystem.cpp \
playerMovementSystem.cpp \
randomGeneration.cpp \
main.cpp

srcs_cpp := ${src_cpp_names:%=${src_dir}/%}
src_c_names := shaders.c tex_cami.c
srcs_c := ${src_c_names:%=${src_dir}/%}

header_names := \
arraySlice.hpp \
camiCubeSystem.hpp \
camiCubeVao.hpp \
camiCubeOrientation.hpp \
chunk.hpp \
chunkGrid.hpp \
chunkingSystem.hpp \
collision.hpp \
collisionBroadSystem.hpp \
doubleBuffer.hpp \
edgeDetectorSystem.hpp \
fixedPackedArray.hpp \
meshSphere.hpp \
inputEvent.hpp \
inputSystem.hpp \
playerControlState.hpp \
playerControlSystem.hpp \
playerMovementSystem.hpp \
randomGeneration.hpp \
shaders.h \
textures.h \
world.hpp

headers := ${header_names:%=${src_dir}/%}

shader_dir := ./src/shaders
shader_names := \
camiCube.frag \
camiCube.vert \
sphere.frag \
sphere.vert \
highlight.frag \
highlight.vert
shaders := ${shader_names:%=${shader_dir}/%}

shader_dumps := ${shaders:%=%.xxd}

object_names := ${src_cpp_names:%.cpp=%.cpp.o} ${src_c_names:%.c=%.c.o}
objects := ${object_names:%=${dir_build}/%}

include_dir := ./external/include
include_options := -I${include_dir}

compile_options := -c -g ${include_options}

lib_dir := ./external/lib
lib_names := glfw3 GLEW GL m
libs := ${lib_names:%=-l%}
lib_options := -L${lib_dir} ${libs}



${dir_build}/${program_name}: ${objects}
	g++ -o ${dir_build}/${program_name} ${objects} ${lib_options}

# Compile .cpp sources
${filter %.cpp.o,${objects}}: ${dir_build}/%.cpp.o: ${src_dir}/%.cpp ${headers}
	g++ ${compile_options} -o $@ $<

# Compile .c sources
${filter %.c.o,${objects}}: ${dir_build}/%.c.o: ${src_dir}/%.c
	gcc ${compile_options} -o $@ $<

# Compile shaders.c depends on shader dumps
${dir_build}/shaders.c.o: ${src_dir}/shaders.c ${shader_dumps}

# Create shader dumps
${shader_dumps}: %.xxd: %
	xxd -i < $< > $@

.PHONY: clean
clean:
	rm -f ${dir_build}/*.o
	rm -f ${dir_build}/${program_name}
	rm -f ${shader_dir}/*.xxd

.PHONY: wc
wc:
	wc -l ${srcs_cpp} ${srcs_c} ${headers} ${shaders}