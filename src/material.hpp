#ifndef _MATERIAL_HPP
#define _MATERIAL_HPP

#include "gl_includes.h"

#include <variant>

namespace GlWorld {

class Scene;
class WorldObject;
class Mesh;

class Material
{

public:

    virtual void bindMesh(
        Mesh &mesh
    ) = 0;

    virtual void activate(
        const Scene &scene,
        const Transform& transformModel
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

    void bindMesh(
        Mesh &mesh
    );

    void activate(
        const Scene &scene,
        const Transform& transformModel
    );

    void deactivate(void);

private:

    GLuint texture;
    GLuint uniformMatrix;
    GLuint uniformTexture;
    GLuint shaderProgram;

    GLuint attributePosition4d;
    GLuint attributeTextureCoords2d;

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

    void bindMesh(
        Mesh &mesh
    );

    void activate(
        const Scene &scene,
        const Transform& transformModel
    );

    void deactivate(void);

private:

    GLuint shaderProgram;
    GLuint attribute4dPosition;
    GLuint attribute2dTextureCoords;
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