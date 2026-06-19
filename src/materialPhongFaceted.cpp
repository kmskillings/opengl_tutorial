#include <variant>
#include <memory>

#include "gl_includes.h"
#include "worldObject.hpp"
#include "light.hpp"
#include "scene.hpp"
#include "materialPhongFaceted.hpp"

namespace GlWorld
{

MaterialPhongFaceted::MaterialPhongFaceted(
    GLuint shaderProgram,
    std::variant<GLuint, glm::vec3> diffuse,
    std::variant<GLuint, glm::vec3> specular,
    std::variant<GLuint, float> specularExp,
    std::variant<GLuint, glm::vec3> emissive
)
{
    this->shaderProgram = shaderProgram;

    this->uniformMatrixModel = glGetUniformLocation(this->shaderProgram, "matrixModel");
    this->uniformMatrixView = glGetUniformLocation(this->shaderProgram, "matrixView");
    this->uniformMatrixProjection = glGetUniformLocation(this->shaderProgram, "matrixProjection");

    this->uniformDiffuse = glGetUniformLocation(this->shaderProgram, "textureDiffuse");
    this->uniformSpecular = glGetUniformLocation(this->shaderProgram, "textureSpecular");
    this->uniformSpecularExp = glGetUniformLocation(this->shaderProgram, "textureSpecularExp");
    this->uniformEmissive = glGetUniformLocation(this->shaderProgram, "textureEmissive");

    this->uniformAmbientColor = glGetUniformLocation(this->shaderProgram, "ambientColor");
    this->uniformLightDirection = glGetUniformLocation(this->shaderProgram, "lightDirection");
    this->uniformLightColor = glGetUniformLocation(this->shaderProgram, "lightColor");

    this->diffuse = assignTexture(diffuse);
    this->specular = assignTexture(specular);
    this->specularExp = assignTexture(specularExp);
    this->emissive = assignTexture(emissive);
    
}

GLuint MaterialPhongFaceted::getLocationPosition(void) const
{
    return glGetAttribLocation(this->shaderProgram, "position");
}

GLuint MaterialPhongFaceted::getLocationTextureCoords(void) const
{
    return glGetAttribLocation(this->shaderProgram, "textureCoords");
}

void MaterialPhongFaceted::activate(
    const WorldObject &worldObject,
    const Scene &scene
)
{
    
    glUseProgram(this->shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->specular);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, this->specularExp);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, this->emissive);

    glm::mat4 matrixModel = worldObject.getTransform()->getMatrixModel();
    glUniformMatrix4fv(
        this->uniformMatrixModel,
        1,
        GL_FALSE,
        glm::value_ptr(matrixModel)
    );
    glm::mat4 matrixView = scene.getCamera()->getMatrixView();
    glUniformMatrix4fv(
        this->uniformMatrixView,
        1,
        GL_FALSE,
        glm::value_ptr(matrixView)
    );
    glm::mat4 matrixProjection = scene.getCamera()->getMatrixProject();
    glUniformMatrix4fv(
        this->uniformMatrixProjection,
        1,
        GL_FALSE,
        glm::value_ptr(matrixProjection)
    );
    
    glUniform1i(this->uniformDiffuse, 0);
    glUniform1i(this->uniformSpecular, 1);
    glUniform1i(this->uniformSpecularExp, 2);
    glUniform1i(this->uniformEmissive, 3);

    glUniform3fv(this->uniformAmbientColor, 1, glm::value_ptr(scene.getLightAmbient()->getColor()));
    glm::vec3 lightDirection = glm::vec3(matrixView * scene.getLightDirectional()->getDirection());
    glUniform3fv(this->uniformLightDirection, 1, glm::value_ptr(lightDirection));
    glUniform3fv(this->uniformLightColor, 1, glm::value_ptr(scene.getLightDirectional()->getColor()));

}

void MaterialPhongFaceted::deactivate(void)
{
    glUseProgram(0);
}

GLuint assignTexture(std::variant<GLuint, glm::vec3> v)
{
    GLuint* texturePtr = std::get_if<GLuint>(&v);
    if (texturePtr == nullptr)
    {
        glm::vec3 textureColor = std::get<glm::vec3>(v);
        return create1x1Texture(
            GL_RGB, 
            GL_RGB, 
            GL_FLOAT, 
            glm::value_ptr(textureColor)
        );
    } else {
        return *texturePtr;
    }
}

GLuint assignTexture(std::variant<GLuint, float> v)
{
    GLuint* texturePtr = std::get_if<GLuint>(&v);
    if (texturePtr == nullptr)
    {
        float textureLevel = std::get<float>(v);
        return create1x1Texture(
            GL_RED, 
            GL_RED, 
            GL_FLOAT, 
            &textureLevel
        );
    } else {
        return *texturePtr;
    }
}

GLuint create1x1Texture(
    GLuint internalFormat,
    GLuint format,
    GLuint type,
    void* data
)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        1,
        1,
        0,
        format,
        type,
        data
    );
    return texture;
    glBindTexture(GL_TEXTURE_2D, 0);
}

}