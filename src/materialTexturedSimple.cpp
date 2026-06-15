#include "materialTexturedSimple.hpp"

namespace GlWorld
{

MaterialTexturedSimple::MaterialTexturedSimple(
    GLuint shaderProgram
)
{
    this->shaderProgram = shaderProgram;
    glBindFragDataLocation(this->shaderProgram, 0, "outColor");
}

void MaterialTexturedSimple::setMatrix(
    const glm::mat4 &matrix
)
{
    this->matrix = glm::mat4(matrix);
}

void MaterialTexturedSimple::setTexture(
    GLuint texture
)
{
    this->texture = texture;
}

GLuint MaterialTexturedSimple::getLocationPosition(void) const
{
    GLuint location = glGetAttribLocation(this->shaderProgram, "position");
    GLuint error = glGetError();
    return location;
}

GLuint MaterialTexturedSimple::getLocationTextureCoords(void) const
{
    return glGetAttribLocation(this->shaderProgram, "textureCoords");
}

void MaterialTexturedSimple::activate(void)
{
    glUseProgram(this->shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    GLuint uniformTexture = glGetUniformLocation(this->shaderProgram, "textureMain");
    glUniform1i(uniformTexture, 0);
    GLuint uniformMatrix = glGetUniformLocation(this->shaderProgram, "transform");
    glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, glm::value_ptr(this->matrix));
}

}