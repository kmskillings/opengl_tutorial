#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include <chrono>
#include <cmath>
#include <memory>

#include "textures.h"
#include "shaders.h"

#include "scene.hpp"
#include "camera.hpp"
#include "transform.hpp"
#include "model.hpp"
#include "material.hpp"
#include "meshTextured.hpp"
#include "worldObject.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Cami Cube"

#define THROB_PERIOD_MILLIS 2000

GLFWwindow* setupGl(void);
uint64_t millisSinceEpoch(void);

int main(void)
{

    constexpr float PI_F = 3.1415927f;

    // Setup code
    GLFWwindow* window = setupGl();
    glfwMakeContextCurrent(window);
    glewInit();

    // Set up scene

    // Create camera
    std::shared_ptr<GlWorld::Transform> cameraTransform = std::make_shared<GlWorld::Transform>();
    std::shared_ptr<GlWorld::Camera> camera = std::make_shared<GlWorld::Camera>(
        cameraTransform,
        PI_F / 4,
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        0.1f,
        10.0f
    );

    // Create Cami Cube
    // First load the Cami texture
    GLuint textureCami;
    glGenTextures(1, &textureCami);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureCami);
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_BGRA, 
        CAMI_TEXTURE_WIDTH, 
        CAMI_TEXTURE_HEIGHT, 
        0, 
        GL_BGRA, 
        GL_UNSIGNED_BYTE, 
        camiTextureBytes
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Then create the shader program
    GLint shaderStatus;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderStatus);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderStatus);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetShaderiv(fragmentShader, GL_LINK_STATUS, &shaderStatus);

    // Create the material
    std::shared_ptr<GlWorld::MaterialTexturedSimple> material = std::make_shared<GlWorld::MaterialTexturedSimple>(shaderProgram, textureCami);

    // Create the mesh
    std::shared_ptr<GlWorld::MeshTextured> meshCube = GlWorld::MeshTextured::sphere(1.0f, 2, 4);

    // Create the model
    std::shared_ptr<GlWorld::ModelTexturedSimple> modelCami = std::make_shared<GlWorld::ModelTexturedSimple>(meshCube, material);

    // Create the WorldObject
    std::shared_ptr<GlWorld::WorldObject> woCami = std::make_shared<GlWorld::WorldObject>(modelCami);

    // Create and populate the scene
    glm::vec4 skyColor = glm::vec4(0.5f, 0.8f, 1.0f, 1.0f);
    // std::unique_ptr<GlWorld::Scene> scene = std::make_unique<GlWorld::Scene>(camera, skyColor);
    // scene.get()->addWorldObject(woCami);

    // Position the camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 cameraAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    float angle = atan2(-cameraPos.y, cameraPos.z);
    cameraTransform.get()->setPosition(cameraPos);
    cameraTransform.get()->setRotation(angle, cameraAxis);

    glm::vec3 axisWoCami = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
    float rotationsPerSecond = 0.2f;
    float radiansPerMilli = 2.0f * PI_F * rotationsPerSecond / 1000.0f;

    uint64_t millisStart = millisSinceEpoch();

    // Game loop
    while(glfwWindowShouldClose(window) == GL_FALSE) {

        uint64_t millisFrame = millisSinceEpoch();
        int millisElapsed = millisFrame - millisStart;
        float angle = radiansPerMilli * millisElapsed;
        woCami.get()->getTransform().get()->setRotation(angle, axisWoCami);

        // Draw the scene
        // scene.get()->draw();
        glfwSwapBuffers(window);

        // Handle input
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

    }

}

GLFWwindow* setupGl(void) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    return glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

}

uint64_t millisSinceEpoch()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}