#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <chrono>
#include <stdio.h>

#include "world.hpp"
#include "camiCubeVao.hpp"
#include "meshSphere.hpp"
#include "chunkingStrategy.hpp"
#include "inputEvent.hpp"
#include "inputSystem.hpp"
#include "exitGameControlSystem.hpp"
#include "playerControlState.hpp"
#include "playerControlSystem.hpp"
#include "playerMovementSystem.hpp"
#include "camiCubeSystem.hpp"
#include "chunkingSystem.hpp"
#include "collision.hpp"
#include "collisionBroadSystem.hpp"
#include "camiCubeTransformSystem.hpp"
#include "collisionMediumSystem.hpp"
#include "collisionNarrowSystem.hpp"
#include "edgeDetectorSystem.hpp"
#include "highlightSystem.hpp"
#include "instanceAttributeSystem.hpp"
#include "sphereResizingSystem.hpp"
#include "fullscreenQuadVao.hpp"
#include "mouseModeSwitchSystem.hpp"
#include "sphereOrientationSystem.hpp"
#include "texture.hpp"
#include "shaderManager.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Cami Cube"

constexpr float cloudRadius = 40.0f;
constexpr float chunkSideLength = 5.0f;
constexpr uint32_t chunkCountAxis = 2 * static_cast<int>(ceil(cloudRadius / chunkSideLength));
#define CLOUD_ROTATION_RATE_MAX 1.0f
constexpr uint averageCamiCubesPerChunk = 10;

constexpr float cameraSpeedTranslation = 5.0f;
constexpr float cameraSensitivityPitch = 0.005f;
constexpr float cameraSensitivityYaw = 0.005f;
constexpr float cameraSpeedRoll = 1.0f;
constexpr float cameraStandoff = 2.0f;

constexpr float sphereRadiusInitial = 0.5f;
constexpr float sphereResizeSensitivity = 0.2;
constexpr float sphereRadiusMin = 0.01f;
constexpr float sphereRadiusMax = 3.0f / 4.0f * chunkSideLength;

constexpr float throbPeriod = 1.0f;

constexpr uint camiCubeCountMax = 10000;
constexpr uint32_t collisionsBroadCountMax = 1024;
constexpr uint32_t collisionsMediumCountMax = 1024;
constexpr uint32_t collisionsNarrowCountMax = 1024;

constexpr glm::vec3 cubeHighlightColor = glm::vec3(0.8f, 1.0f, 0.8f);
constexpr glm::vec3 cutoutHighlightColor = glm::vec3(0.5f, 1.0f, 0.5f);
constexpr glm::vec3 sphereWireframeColor = glm::vec3(1.0f, 0.0f, 0.0f);

constexpr int randomSeed = 11141997;

GLFWwindow* setupGl(void);
double secondsSinceEpoch(void);

glm::quat updatePlayerOrientation(
    const glm::quat& playerOrientation,
    GLFWwindow* window,
    const float& secondsDelta,
    const double& mouseXDelta,
    const double& mouseYDelta
);

glm::vec3 updatePlayerPosition(
    const glm::vec3& position,
    const glm::quat& orientation,
    GLFWwindow* window,
    const float& secondsDelta
);

int main(void)
{

    GLuint error;

    // Setup code
    GLFWwindow* window = setupGl();
    glfwMakeContextCurrent(window);
    glewInit();

    Texture::init();

    World world;
    ChunkingSystem chunkingSystem;
    CamiCubeSystem camiCubeSystem;
    PlayerControlSystem playerControlSystem;
    PlayerMovementSystem playerMovementSystem(
        cameraSpeedTranslation,
        cameraSensitivityPitch,
        cameraSpeedRoll
    );
    MouseModeSwitchSystem MouseModeSwitchSystem(
        GLFW_KEY_LEFT_CONTROL
    );
    SphereOrientationSystem sphereOrientaitonSystem(
        cameraSensitivityPitch,
        cameraSpeedRoll
    );
    SphereResizingSystem sphereResizingSystem(
        sphereRadiusMin,
        sphereRadiusMax,
        sphereResizeSensitivity
    );
    ExitGameControlSystem exitGameControlSystem(window);
    InputSystem inputSystem;
    CollisionBroadSystem collisionBroadSystem;
    CollisionMediumSystem collisionMediumSystem;
    CamiCubeTransformSystem camiCubeTransformSystem;
    CollisionNarrowSystem collisionNarrowSystem;
    EdgeDetectorSystem<Collision> collisionEdgeDetectorSystem;
    HighlightSystem highlightSystem(
        sizeof(CamiCubeVao::CamiCubeInstance),
        offsetof(CamiCubeVao::CamiCubeInstance, color),
        cubeHighlightColor
    );
    InstanceAttributeSystem camiCubeAttributeSystem;

    world.init(
        camiCubeCountMax,
        cloudRadius,
        chunkCountAxis,
        sphereRadiusInitial,
        128,
        collisionsBroadCountMax,
        collisionsMediumCountMax,
        collisionsNarrowCountMax
    );
    chunkingSystem.init(
        camiCubeCountMax,
        world.chunks.capacity
    );
    camiCubeSystem.init(randomSeed);
    inputSystem.init(window);

    camiCubeSystem.populateCamiCubes(
        camiCubeCountMax,
        cloudRadius,
        1.0f,
        world.camiCubePositions.now(),
        world.camiCubeOrientations.now()
    );

    chunkingSystem.calculateChunks(
        world.chunkGrid,
        world.chunks,
        world.camiCubePositions.now()
    );
    chunkingSystem.chunkArray(
        world.camiCubePositions
    );
    chunkingSystem.chunkArray(
        world.camiCubeOrientations
    );

    CamiCubeVao camiCubeVao(
        camiCubeCountMax,
        world.camiCubePositions.now().data,
        world.camiCubeOrientations.now().data
    );

    MeshSphere meshSphere(16, 32, 1.0f);

    FullscreenQuadVao fullscreenQuadVao;

    ShaderManager shaderManager;
    shaderManager.init("resources/shaders");

    GLuint camiCubeVert = shaderManager.loadGlslSource("camiCube.vert");
    GLuint camiCubeFrontFrag = shaderManager.loadGlslSource("camiCubeFront.frag");
    GLuint camiCubeBackFrag = shaderManager.loadGlslSource("camiCubeBack.frag");
    GLuint sphereVert = shaderManager.loadGlslSource("sphere.vert");
    GLuint sphereLinesFrag = shaderManager.loadGlslSource("sphereLines.frag");
    GLuint sphereTrianglesFrag = shaderManager.loadGlslSource("sphereTriangles.frag");
    GLuint highlightVert = shaderManager.loadGlslSource("highlight.vert");
    GLuint highlightFrag = shaderManager.loadGlslSource("highlight.frag");

    GLuint camiCubeFrontShader = shaderManager.compileShader(
        &camiCubeVert, 1,
        &camiCubeFrontFrag, 1
    );

    GLuint camiCubeBackShader = shaderManager.compileShader(
        &camiCubeVert, 1,
        &camiCubeBackFrag, 1
    );

    GLuint sphereTrianglesShader = shaderManager.compileShader(
        &sphereVert, 1,
        &sphereTrianglesFrag, 1
    );

    GLuint sphereLinesShader = shaderManager.compileShader(
        &sphereVert, 1,
        &sphereLinesFrag, 1
    );

    GLuint highlightShader = shaderManager.compileShader(
        &highlightVert, 1,
        &highlightFrag, 1
    );
    
    Texture camiCubeTexture = Texture::load("cami.bmp");
    Texture sphereTexture = Texture::load("gracie.bmp");

    glm::quat cameraOrientation = glm::angleAxis(
        0.0f, 
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    // Calculate project matrix
    glm::mat4 matrixProject = glm::perspective(
        (float)(M_PI / 3.0f),
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        0.1f,
        10000.0f
    );

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);

    double mouseXNow;
    double mouseYNow;
    double mouseXLast;
    double mouseYLast;
    double mouseXDelta;
    double mouseYDelta;
    glfwGetCursorPos(window, &mouseXNow, &mouseYNow);

    double secondsNow = secondsSinceEpoch();
    double secondsLast;
    double secondsDelta;
    double secondsElapsed = 0;

    while(glfwWindowShouldClose(window) == GL_FALSE) {

        secondsLast = secondsNow;
        secondsNow = secondsSinceEpoch();
        secondsDelta = secondsNow - secondsLast;
        secondsElapsed = secondsElapsed + secondsDelta;

        inputSystem.getInputs(world.inputEvents);
        playerControlSystem.update(
            world.inputEvents,
            world.playerControlState
        );
        exitGameControlSystem.updateWindowShouldClose(world.inputEvents);
        MouseModeSwitchSystem.setMouseMode(
            world.inputEvents,
            world.cameraOrientationActive,
            world.sphereOrientationActive
        );
        playerMovementSystem.update(
            secondsDelta,
            world.playerControlState,
            world.cameraOrientationActive,
            world.spherePosition,
            world.cameraOrientation
        );
        sphereOrientaitonSystem.updateSphereOrientation(
            secondsDelta,
            world.playerControlState,
            world.sphereOrientationActive,
            world.cameraOrientation,
            world.sphereOrientation
        );
        sphereResizingSystem.resizeSphere(
            world.inputEvents,
            world.sphereRadius
        );
        collisionBroadSystem.detectCollisions(
            world.spherePosition,
            world.chunkGrid,
            world.chunks,
            world.collisionsBroad
        );
        collisionMediumSystem.detectCollisions(
            sqrt(1.5f),
            world.sphereRadius,
            world.spherePosition,
            world.camiCubePositions.now(),
            world.collisionsBroad,
            world.collisionsMedium
        );
        camiCubeTransformSystem.calculateTransforms(
            secondsElapsed,
            world.collisionsMedium,
            world.camiCubePositions.now(),
            world.camiCubeOrientations.now(),
            world.camiCubeTransforms
        );
        world.collisionsNarrow.swap();
        collisionNarrowSystem.detectCollisions(
            1.0f,
            world.sphereRadius,
            world.spherePosition,
            world.camiCubeTransforms,
            world.collisionsMedium,
            world.collisionsNarrow.now()
        );
        collisionEdgeDetectorSystem.detectEdges(
            world.collisionsNarrow,
            world.appearedCollisions,
            world.disappearedCollisions
        );
        world.camiCubeUpdateAttributes.clear();
        world.camiCubeUpdateIndexes.clear();
        world.camiCubeUpdateData.clear();
        highlightSystem.getColorUpdates(
            world.appearedCollisions,
            world.disappearedCollisions,
            world.camiCubeUpdateAttributes,
            world.camiCubeUpdateIndexes,
            world.camiCubeUpdateData
        );
        camiCubeAttributeSystem.updateAttributes(
            camiCubeVao.getVao(),
            camiCubeVao.getInstanceVbo(),
            world.camiCubeUpdateAttributes,
            world.camiCubeUpdateIndexes,
            world.camiCubeUpdateData
        );

        for (int i = 0; i < world.inputEvents.count; i++)
        {
            const InputEvent& inputEvent = world.inputEvents[i];
            if (std::holds_alternative<MouseButtonEvent>(inputEvent))
            {
                const MouseButtonEvent& mbEvent 
                    = std::get<MouseButtonEvent>(inputEvent);

                printf(
                    "Mouse button event with button %i and action %i detected.\n",
                    mbEvent.button,
                    mbEvent.action
                );
            }
        }

        // Calculate the view matrix
        glm::mat4 matrixRotate = glm::mat4_cast(world.cameraOrientation);
        glm::mat4 matrixStandoff = glm::translate(
            glm::mat4(1.0f), 
            glm::vec3(0.0f, 0.0f, cameraStandoff)
        );
        glm::mat4 matrixPosition = glm::translate(
            glm::mat4(1.0f),
            world.spherePosition
        );
        glm::mat4 matrixView = glm::inverse(
            matrixPosition * 
            matrixRotate * 
            matrixStandoff
        );

        glm::mat4 matrixProjView = matrixProject * matrixView;

        glm::mat4 matrixSphereModel = glm::mat4(1.0f);
        matrixSphereModel = glm::translate(matrixSphereModel, world.spherePosition);
        matrixSphereModel = matrixSphereModel * glm::mat4_cast(world.sphereOrientation);
        matrixSphereModel = glm::scale(
            matrixSphereModel, 
            glm::vec3(world.sphereRadius)
        );
        glm::mat4 matrixSphere = matrixProjView * matrixSphereModel;

        // Rendering
        // Clear the buffer
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT
        );

        // Draw the Cami Cubes
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, camiCubeTexture.id);
        glUseProgram(camiCubeFrontShader);
        glUniformMatrix4fv(
            0,
            1,
            GL_FALSE,
            glm::value_ptr(matrixProjView)
        );
        glUniform1f(
            1,
            secondsElapsed
        );
        glUniform1i(
            2,
            0
        );
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ZERO);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDisable(GL_STENCIL_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glBindVertexArray(camiCubeVao.getVao());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, camiCubeVao.getEbo());
        glDrawElementsInstanced(
            GL_TRIANGLES,
            3 * camiCubeVao.getTriangleCount(),
            GL_UNSIGNED_INT,
            (void*)0,
            camiCubeVao.getInstanceCount()
        );
        glUseProgram(camiCubeBackShader);
        glUniformMatrix4fv(
            0,
            1,
            GL_FALSE,
            glm::value_ptr(matrixProjView)
        );
        glUniform1f(
            1,
            secondsElapsed
        );
        glUniform1i(
            2,
            0
        );
        glCullFace(GL_FRONT);
        glDrawElementsInstanced(
            GL_TRIANGLES,
            3 * camiCubeVao.getTriangleCount(),
            GL_UNSIGNED_INT,
            (void*)0,
            camiCubeVao.getInstanceCount()
        );

        // Highlight any areas that are inside the sphere.
        glUseProgram(sphereTrianglesShader);
        glUniformMatrix4fv(
            0,
            1,
            GL_FALSE,
            glm::value_ptr(matrixSphere)
        );
        glBindVertexArray(meshSphere.getVao());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshSphere.getEboTriangles());
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glDrawElements(
            GL_TRIANGLES,
            3 * meshSphere.getCountTriangles(),
            GL_UNSIGNED_INT,
            (void*)0
        );
        glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
        glCullFace(GL_BACK);
        glDrawElements(
            GL_TRIANGLES,
            3 * meshSphere.getCountTriangles(),
            GL_UNSIGNED_INT,
            (void*)0
        );
        glUseProgram(highlightShader);
        glUniform3fv(
            0, 
            1,
            glm::value_ptr(cutoutHighlightColor)
        );
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glBlendFunc(GL_DST_COLOR, GL_ZERO);
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glCullFace(GL_BACK);
        glBindVertexArray(fullscreenQuadVao.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fullscreenQuadVao.eboTriangles);
        glDrawElements(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            (void*)0
        );

        // Draw the sphere

        glBindVertexArray(meshSphere.getVao());
        glBlendFunc(GL_ONE, GL_ZERO);
        glDepthMask(GL_TRUE);
        glDisable(GL_STENCIL_TEST);
        if (world.sphereOrientationActive)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshSphere.getEboTriangles());
            glBindTexture(GL_TEXTURE_2D, sphereTexture.id);
            glUseProgram(sphereTrianglesShader);
            glUniformMatrix4fv(
                0,
                1,
                GL_FALSE,
                glm::value_ptr(matrixSphere)
            );
            glUniform1i(
                1,
                0
            );
            glDrawElements(
                GL_TRIANGLES,
                3 * meshSphere.getCountTriangles(),
                GL_UNSIGNED_INT,
                (void*)0
            );
        }
        else
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshSphere.getEboLines());
            glUseProgram(sphereLinesShader);
            glUniformMatrix4fv(
                0,
                1,
                GL_FALSE,
                glm::value_ptr(matrixSphere)
            );
            glUniform3fv(
                1,
                1,
                glm::value_ptr(sphereWireframeColor)
            );
            glDrawElements(
                GL_LINES,
                2 * meshSphere.getCountLines(),
                GL_UNSIGNED_INT,
                (void*)0
            );
        }

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