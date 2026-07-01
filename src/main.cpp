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
#include "light.hpp"
#include "mesh.hpp"
#include "motion.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Cami Cube"

#define CLOUD_RADIUS 10.0f
#define CLOUD_ROTATION_RATE_MAX 0.1f

#define CAMERA_SPEED_TRANSLATION 5.0f
#define CAMERA_SENSITIVITY_PITCH 0.1f
#define CAMERA_SENSITIVITY_YAW   0.1f

#define THROB_PERIOD_MILLIS 2000

GLFWwindow* setupGl(void);

double secondsSinceEpoch(void);

void reportShaderStatus(GLuint shader);

class CamiCube : public GlWorld::SceneElement
{

public:

    CamiCube(
        std::shared_ptr<GlWorld::Mesh> mesh,
        std::shared_ptr<GlWorld::Material> material,
        float* transformData
    )
    {
        glm::vec3 position = glm::vec3(
            transformData[0], 
            transformData[1], 
            transformData[2]
        );
        position = position * CLOUD_RADIUS;

        float orientationAngle = 
            transformData[3];
        glm::vec3 orientationAxis = glm::vec3(
            transformData[4],
            transformData[5],
            transformData[6]
        );

        float rotationAngle = transformData[7] * CLOUD_ROTATION_RATE_MAX;
        glm::vec3 rotationAxis = glm::vec3(
            transformData[8],
            transformData[9],
            transformData[10]
        );

        std::shared_ptr<GlWorld::Transform> transform
            = std::make_unique<GlWorld::Transform>();
        transform->setPosition(position);
        transform->setRotation(orientationAngle, orientationAxis);

        this->model = std::make_shared<GlWorld::Model>(
            transform,
            material,
            mesh
        );

        this->motion = std::make_shared<GlWorld::MotionRotate>(
            transform,
            rotationAngle,
            rotationAxis
        );
        
    }

    bool caresAboutUpdatePhysical(void) const
    {
        return 
            this->model->caresAboutUpdatePhysical() ||
            this->motion->caresAboutUpdatePhysical()    
        ;
    }

    bool caresAboutUpdateVisual(void) const
    {
        return 
            this->model->caresAboutUpdateVisual() ||
            this->motion->caresAboutUpdateVisual()
        ;
    }

    bool caresAboutRenderPass(void) const
    {
        return 
            this->model->caresAboutRenderPass() ||
            this->motion->caresAboutRenderPass()
        ;
    }

    void updatePhysicalPre(
        const GlWorld::Scene& scene,
        const float& secondsDelta
    )
    {
        if (this->model->caresAboutUpdatePhysical())
            this->model->updatePhysicalPre(scene, secondsDelta);
        if (this->motion->caresAboutUpdatePhysical())
            this->motion->updatePhysicalPre(scene, secondsDelta);
    }

    void updateVisualPre(
        const GlWorld::Scene& scene,
        const float& secondsDelta
    )
    {
        if (this->model->caresAboutUpdateVisual())
            this->model->updateVisualPre(scene, secondsDelta);
        if (this->motion->caresAboutUpdateVisual())
            this->motion->updateVisualPre(scene, secondsDelta);
    }

    void updatePhysical(
        const GlWorld::Scene& scene,
        const float& secondsDelta
    )
    {
        if (this->model->caresAboutUpdatePhysical())
            this->model->updatePhysical(scene, secondsDelta);
        if (this->motion->caresAboutUpdatePhysical())
            this->motion->updatePhysical(scene, secondsDelta);
    }

    void updateVisual(
        const GlWorld::Scene& scene,
        const float& secondsDelta
    )
    {
        if (this->model->caresAboutUpdateVisual())
            this->model->updateVisual(scene, secondsDelta);
        if (this->motion->caresAboutUpdateVisual())
            this->motion->updateVisual(scene, secondsDelta);
    }

    void updatePhysicalPost(
        const GlWorld::Scene& scene,
        const float& secondsDelta
    )
    {
        if (this->model->caresAboutUpdatePhysical())
            this->model->updatePhysicalPost(scene, secondsDelta);
        if (this->motion->caresAboutUpdatePhysical())
            this->motion->updatePhysicalPost(scene, secondsDelta);
    }

    void updateVisualPost(
        const GlWorld::Scene& scene,
        const float& secondsDelta
    )
    {
        if (this->model->caresAboutUpdateVisual())
            this->model->updateVisualPost(scene, secondsDelta);
        if (this->motion->caresAboutUpdateVisual())
            this->motion->updateVisualPost(scene, secondsDelta);
    }

    void draw(
        const GlWorld::Scene& scene
    )
    {
        if (this->model->caresAboutRenderPass())
            this->model->draw(scene);
        if (this->motion->caresAboutRenderPass())
            this->motion->draw(scene);
    }

private:

    std::shared_ptr<GlWorld::Model> model;

    std::shared_ptr<GlWorld::MotionRotate> motion;

};

int main(void)
{

    GLuint error;

    // Setup code
    GLFWwindow* window = setupGl();
    glfwMakeContextCurrent(window);
    glewInit();

    // Create camera

    std::shared_ptr<GlWorld::Transform> cameraTransform 
        = std::make_shared<GlWorld::Transform>();
    std::shared_ptr<GlWorld::Camera> camera 
        = std::make_shared<GlWorld::Camera>(
            cameraTransform,
            M_PI / 4,
            (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
            0.1f,
            100.0f
        );

    std::shared_ptr<GlWorld::LightAmbient> lightAmbient 
        = std::make_shared<GlWorld::LightAmbient>(
            glm::vec3(0.1f, 0.1f, 0.1f)
        );

    std::shared_ptr<GlWorld::Transform> transformLight 
        = std::make_shared<GlWorld::Transform>();

    transformLight->rotate(-(float)M_PI / 4.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    
    std::shared_ptr<GlWorld::LightDirectional> lightDirectional 
        = std::make_shared<GlWorld::LightDirectional>(
            transformLight, 
            glm::vec3(0.9, 0.9, 0.9)
        );

    glm::vec4 skyColor = glm::vec4(0.5f, 0.8f, 1.0f, 1.0f);
    
    std::unique_ptr<GlWorld::Scene> scene 
        = std::make_unique<GlWorld::Scene>(
            camera, 
            skyColor, 
            lightAmbient, 
            lightDirectional
        );

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
    std::shared_ptr<GlWorld::MaterialPhongFaceted> material 
        = std::make_shared<GlWorld::MaterialPhongFaceted>(
            shaderProgram,
            textureCami,
            glm::vec3(0.1f, 0.1f, 0.1f),
            16.0f,
            glm::vec3(0.0f, 0.0f, 0.0f)
        );

    // Create the mesh
    std::shared_ptr<GlWorld::MeshTextured> meshCube = 
        GlWorld::MeshTextured::cube(1.0f);

    float transformData[] = {
        #include "cloud.txt"
    };
    int transformCount = sizeof(transformData) / sizeof(float) / 11;
    for (int i = 0; i < transformCount; i++)
    {
        std::shared_ptr<CamiCube> cc = std::make_shared<CamiCube>(
            meshCube,
            material,
            transformData + 11 * i
        );
        scene->addElement(cc);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    bool mouseInitialized = false;
    double mousePositionNowX;
    double mousePositionNowY;
    glfwGetCursorPos(window, &mousePositionNowX, &mousePositionNowY);
    double mousePositionLastX;
    double mousePositionLastY;

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

        // Translation controls
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

        // Camera view controls
        mousePositionLastX = mousePositionNowX;
        mousePositionLastY = mousePositionNowY;
        glfwGetCursorPos(window, &mousePositionNowX, &mousePositionNowY);
        double mousePositionDeltaX = mousePositionNowX - mousePositionLastX;
        double mousePositionDeltaY = mousePositionNowY - mousePositionLastY;

        glm::vec3 cameraPitchAxis = glm::vec3(
            camera->getTransform()->getMatrixModel() 
            * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)
        );
        camera->getTransform()->rotate(
            -mousePositionDeltaY * CAMERA_SENSITIVITY_PITCH * secondsDelta,
            cameraPitchAxis
        );

        glm::vec3 cameraYawAxis = glm::vec3(
            camera->getTransform()->getMatrixModel()
            * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)
        );
        camera->getTransform()->rotate(
            -mousePositionDeltaX * CAMERA_SENSITIVITY_YAW * secondsDelta,
            cameraYawAxis
        );

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        scene->updatePhysical(secondsDelta);
        scene->updateVisual(secondsDelta);
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

