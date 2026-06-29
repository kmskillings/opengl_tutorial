#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include <chrono>
#include <cmath>
#include <memory>
#include <stdio.h>

extern "C" {
#include "textures.h"
#include "shaders.h"
}

#include "scene.hpp"
#include "camera.hpp"
#include "transform.hpp"
#include "model.hpp"
#include "material.hpp"
#include "worldObject.hpp"
#include "light.hpp"
#include "mesh.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Cami Cube"

#define CLOUD_RADIUS 10.0f
#define CLOUD_ROTATION_RATE_MAX 1.0f

#define CAMERA_SPEED_TRANSLATION 5.0f

#define THROB_PERIOD_MILLIS 2000

GLFWwindow* setupGl(void);
double secondsSinceEpoch(void);
void reportShaderStatus(GLuint shader);

int main(void)
{

    GLuint error;

    // Setup code
    GLFWwindow* window = setupGl();
    glfwMakeContextCurrent(window);
    glewInit();

    // Set up scene

    // Create camera
    std::shared_ptr<GlWorld::Transform> cameraTransform = std::make_shared<GlWorld::Transform>();
    std::shared_ptr<GlWorld::Camera> camera = std::make_shared<GlWorld::Camera>(
        cameraTransform,
        M_PI / 4,
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        0.1f,
        100.0f
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
    const char* fragmentShaderSources[2] = {
        utilsPhongSource,
        fragmentPhongFacetedSource
    };
    const char* vertexShaderSources[1] = {
        vertexPhongFacetedSource
    };
    GLuint shaderProgram = compileShader(
        vertexShaderSources,
        1,
        fragmentShaderSources,
        2
    );

    // Create the material
    std::shared_ptr<GlWorld::MaterialPhongFaceted> material = std::make_shared<GlWorld::MaterialPhongFaceted>(
        shaderProgram,
        textureCami,
        glm::vec3(0.1f, 0.1f, 0.1f),
        16.0f,
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    // Create the mesh
    std::shared_ptr<GlWorld::MeshTextured> meshCube = 
        GlWorld::MeshTextured::cube(1.0f);

    // Create the model
    std::shared_ptr<GlWorld::ModelTexturedSimple> modelCami = std::make_shared<GlWorld::ModelTexturedSimple>(meshCube, material);

    // Create each worldObject

    // Transform data is in the form:
    // posX, posY, posZ, orientationAngle, orientationAxisX, Y, Z, rotationAngle, rotationX, Y, Z
    // The position is scaled by CLOUD_RADIUS. The rotation angle is scaled by CLOUD_ROTATION_RATE_MAX.
    float transformData[] = {
        #include "cloud.txt"
    };
    int transformCount = sizeof(transformData) / sizeof(float) / 11;
    std::shared_ptr<GlWorld::WorldObject> wos[transformCount];
    float transformRotationAngles[transformCount];
    glm::vec3 transformRotationAxes[transformCount];
    for (int i = 0; i < transformCount; i++)
    {
        glm::vec3 position = glm::vec3(
            transformData[11 * i + 0], 
            transformData[11 * i + 1], 
            transformData[11 * i + 2]
        );
        position = position * CLOUD_RADIUS;

        float orientationAngle = 
            transformData[11 * i + 3];
        glm::vec3 orientationAxis = glm::vec3(
            transformData[11 * i + 4],
            transformData[11 * i + 5],
            transformData[11 * i + 6]
        );

        float rotationAngle =
            transformData[11 * i + 7] * CLOUD_ROTATION_RATE_MAX;
        glm::vec3 rotationAxis = glm::vec3(
            transformData[11 * i + 8],
            transformData[11 * i + 9],
            transformData[11 * i + 10]
        );

        std::shared_ptr<GlWorld::Transform> t =
            std::make_shared<GlWorld::Transform>();
        t->setPosition(position);
        t->setRotation(orientationAngle, orientationAxis);
        
        wos[i] = std::make_shared<GlWorld::WorldObject>(
            modelCami,
            t
        );
        transformRotationAngles[i] = rotationAngle;
        transformRotationAxes[i] = rotationAxis;
    }

    // Create the LightAmbient
    std::shared_ptr<GlWorld::LightAmbient> lightAmbient = std::make_shared<GlWorld::LightAmbient>(glm::vec3(0.1f, 0.1f, 0.1f));

    // Create and position the LightDirectional
    std::shared_ptr<GlWorld::Transform> transformLight = std::make_shared<GlWorld::Transform>();
    transformLight->rotate(-(float)M_PI / 4.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    std::shared_ptr<GlWorld::LightDirectional> lightDirectional = std::make_shared<GlWorld::LightDirectional>(transformLight, glm::vec3(0.9, 0.9, 0.9));

    // Create and populate the scene
    glm::vec4 skyColor = glm::vec4(0.5f, 0.8f, 1.0f, 1.0f);
    std::unique_ptr<GlWorld::Scene> scene = std::make_unique<GlWorld::Scene>(camera, skyColor, lightAmbient, lightDirectional);
    for (std::shared_ptr<GlWorld::WorldObject> wo : wos)
    {
        scene->addWorldObject(wo);
    }

    double secondsStart = secondsSinceEpoch();
    double secondsLast = secondsStart;
    double secondsNow = secondsLast;

    // Game loop
    while(glfwWindowShouldClose(window) == GL_FALSE) {

        secondsLast = secondsNow;
        secondsNow = secondsSinceEpoch();
        float secondsDelta = secondsNow - secondsLast;

        // Handle input
        glfwPollEvents();
        glm::vec4 movementNorm = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            movementNorm = movementNorm + glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            movementNorm = movementNorm + glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            movementNorm = movementNorm + glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            movementNorm = movementNorm + glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);
        }
        if (glm::length(movementNorm) > 0.01f) {
            movementNorm = glm::normalize(movementNorm);
        }
        glm::vec4 cameraDirectionWorld = camera->getTransform()->getMatrixModel() * movementNorm;
        glm::vec3 cameraVelocityWorld = glm::vec3(cameraDirectionWorld) * CAMERA_SPEED_TRANSLATION * secondsDelta;
        camera->getTransform()->translate(cameraVelocityWorld);



        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        for (int i = 0; i < transformCount; i++)
        {
            std::shared_ptr<GlWorld::Transform> t = wos[i]->getTransform();
            t->rotate(
                transformRotationAngles[i] * secondsDelta,
                transformRotationAxes[i]
            );
        }        

        // Draw the scene
        scene->draw();
        
        glfwSwapBuffers(window);

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

double secondsSinceEpoch()
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> ds = now.time_since_epoch();
    return ds.count();
}

