#include "shaders.h"

const char vertexShaderSourceArray[] = {
    #include "vertex.xxd"
    , 0x00
};
const char* vertexShaderSource = vertexShaderSourceArray;

const char fragmentShaderSourceArray[] = {
    #include "fragment.xxd"
    , 0x00
};
const char* fragmentShaderSource = fragmentShaderSourceArray;