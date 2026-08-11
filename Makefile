dir_build := ./build
program_name := program

src_dir := ./src

src_cpp_names := \
camiCubeSystem.cpp \
camiCubeVao.cpp \
camiCubeTransformSystem.cpp \
chunkingSystem.cpp \
collisionBroadSystem.cpp \
collisionMediumSystem.cpp \
collisionNarrowSystem.cpp \
exitGameControlSystem.cpp \
highlightSystem.cpp \
meshSphere.cpp \
mouseModeSwitchSystem.cpp \
inputSystem.cpp \
instanceAttributeSystem.cpp \
playerControlSystem.cpp \
playerMovementSystem.cpp \
randomGeneration.cpp \
shaderManager.cpp \
sphereOrientationSystem.cpp \
sphereResizingSystem.cpp \
texture.cpp \
main.cpp

srcs_cpp := ${src_cpp_names:%=${src_dir}/%}
src_c_names := whereami.c
srcs_c := ${src_c_names:%=${src_dir}/%}

header_names := \
arraySlice.hpp \
camiCubeSystem.hpp \
camiCubeVao.hpp \
camiCubeOrientation.hpp \
camiCubeTransformSystem.hpp \
chunk.hpp \
chunkGrid.hpp \
chunkingSystem.hpp \
collision.hpp \
collisionBroadSystem.hpp \
collisionMediumSystem.hpp \
collisionNarrowSystem.hpp \
doubleBuffer.hpp \
edgeDetectorSystem.hpp \
exitGameControlSystem.hpp \
fixedPackedArray.hpp \
fullscreenQuadVao.hpp \
highlightSystem.hpp \
meshSphere.hpp \
mouseModeSwitchSystem.hpp \
inputEvent.hpp \
inputSystem.hpp \
instanceAttribute.hpp \
instanceAttributeSystem.hpp \
playerControlState.hpp \
playerControlSystem.hpp \
playerMovementSystem.hpp \
randomGeneration.hpp \
shaderManager.hpp \
sphereOrientationSystem.hpp \
sphereResizingSystem.hpp \
texture.hpp \
whereami.h \
world.hpp

headers := ${header_names:%=${src_dir}/%}

object_names := ${src_cpp_names:%.cpp=%.cpp.o} ${src_c_names:%.c=%.c.o}
objects := ${object_names:%=${dir_build}/%}

include_dir := ./external/include
include_options := -I${include_dir}

compile_options := -c -g ${include_options}

lib_dir := ./external/lib
lib_names := glfw3 GLEW GL m
libs := ${lib_names:%=-l%}
lib_options := -L${lib_dir} ${libs}

.PHONY: all
all: ${dir_build}/${program_name} ${dir_build}/resources

${dir_build}/${program_name}: ${objects}
	g++ -o ${dir_build}/${program_name} ${objects} ${lib_options}

# Compile .cpp sources
${filter %.cpp.o,${objects}}: ${dir_build}/%.cpp.o: ${src_dir}/%.cpp ${headers}
	g++ ${compile_options} -o $@ $<

# Compile .c sources
${filter %.c.o,${objects}}: ${dir_build}/%.c.o: ${src_dir}/%.c
	gcc ${compile_options} -o $@ $<

# Create shader dumps
${shader_dumps}: %.xxd: %
	xxd -i < $< > $@

# Copy over resources
.PHONY: ${dir_build}/resources
${dir_build}/resources:
	rsync -rupE --delete ./resources ${dir_build} 

.PHONY: clean
clean:
	rm -f ${dir_build}/*.o
	rm -f ${dir_build}/${program_name}
	rm -rf ${dir_build}/resources

.PHONY: wc
wc:
	wc -l ${srcs_cpp} ${srcs_c} ${headers} ${dir_build}/resources/shaders/*