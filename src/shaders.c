#include "shaders.h"

const char vertexShaderSourceArray[] = {
    #include "shaders/vertex.xxd"
    , 0x00
};
const char* vertexShaderSource = vertexShaderSourceArray;

const char fragmentShaderSourceArray[] = {
    #include "shaders/fragment.xxd"
    , 0x00
};
const char* fragmentShaderSource = fragmentShaderSourceArray;