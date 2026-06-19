#ifndef _MATERIAL_HPP
#define _MATERIAL_HPP

#include "gl_includes.h"

#include <variant>

namespace GlWorld {

class Scene;
class WorldObject;

class Material
{

public:

    virtual void activate(
        const Scene &scene,
        const WorldObject &worldObject
    ) = 0;

    virtual void deactivate(void) = 0;

private:

};


class MaterialTexturedSimple : public Material
{

public:

    MaterialTexturedSimple(
        GLuint shaderProgram,
        GLuint texture
    );

    GLuint getLocationPosition(void) const;
    GLuint getLocationTextureCoords(void) const;

    void activate(
        const Scene &scene,
        const WorldObject &worldObject
    );

    void deactivate(void);

private:

    GLuint texture;
    GLuint uniformMatrix;
    GLuint uniformTexture;
    GLuint shaderProgram;

};

class MaterialPhongFaceted : public Material
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