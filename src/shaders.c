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

const char vertexPhongFacetedSourceArray[] = {
    #include "shaders/vertexPhongFaceted.xxd"
    , 0x00
};
const char* vertexPhongFacetedSource = vertexPhongFacetedSourceArray;

const char fragmentPhongFacetedSourceArray[] = {
    #include "shaders/fragmentPhongFaceted.xxd"
    , 0x00
};
const char* fragmentPhongFacetedSource = fragmentPhongFacetedSourceArray;