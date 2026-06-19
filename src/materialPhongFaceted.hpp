#ifndef _MATERIAL_PHONG_FACETED_HPP
#define _MATERIAL_PHONG_FACETED_HPP

#include "gl_includes.h"
#include "worldObject.hpp"
#include "light.hpp"
#include "scene.hpp"

#include <variant>
#include <memory>

namespace GlWorld
{

class MaterialPhongFaceted
{
public:

    MaterialPhongFaceted(
        GLuint shaderProgram,
        std::variant<GLuint, glm::vec3> diffuse,
        std::variant<GLuint, glm::vec3> specular,
        std::variant<GLuint, float> specularExp,
        std::variant<GLuint, glm::vec3> emissive
    );

    GLuint getLocationPosition(void) const;
    GLuint getLocationTextureCoords(void) const;

    void activate(
        const WorldObject &worldObject,
        const Scene &scene
    );

    void deactivate(void);

private:
    GLuint shaderProgram;
    GLuint uniformMatrixModel;
    GLuint uniformMatrixView;
    GLuint uniformMatrixProjection;
    GLuint uniformDiffuse;
    GLuint uniformSpecular;
    GLuint uniformSpecularExp;
    GLuint uniformEmissive;
    GLuint uniformAmbientColor;
    GLuint uniformLightDirection;
    GLuint uniformLightColor;
    GLuint diffuse;
    GLuint specular;
    GLuint specularExp;
    GLuint emissive;
};

}

#endif