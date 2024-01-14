#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "libs/shader.h"
#include "libs/stb_image.h"

struct Camera {
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraFaceDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float cameraSpeed = 2.0f;
};

int display_w, display_h;

bool settingShowHitbox = true;
bool settingShowCollisionbox = false;

bool gameStart = false;

#include "src/game_manager.h"
#include "src/entity_manager.h"

#include "src/systems/render_system.h"
#include "src/systems/update_system.h"
#include "src/systems/input_system.h"

#include "src/entityV2.h"
#include "src/playerV2.h"
#include "src/bullet.h"

Camera camera;


void test(EntityV2* pointer) {
    std::cout << pointer->active << std::endl;
}

EntityV2* entityList[1];
int currentIndex = 0;

void addEntity(EntityV2* entity) {
    entityList[currentIndex] = entity;

    ++currentIndex;
}

int main() {
    if (!glfwInit()) return 1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui Example", NULL, NULL);

    if (!window) {
        glfwTerminate();

        return 1;
    }

    glfwMakeContextCurrent(window);

    glewInit();

    // Shader shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    // Shader bgShader("shaders/background/bgVertexShader.glsl", "shaders/background/bgFragmentShader.glsl");
    // Shader cShader("shaders/collision/collisionVertexShader.glsl", "shaders/collision/collisionFragmentShader.glsl");
    // Shader menuShader("shaders/menu/menuVertexShader.glsl", "shaders/menu/menuFragmentShader.glsl");

    Shader testShader("shaders/test/vertShader.glsl", "shaders/test/fragShader.glsl");

    EntityManager entityManager;

    UpdateSystem updateSystem;
    RenderSystem renderSystem;
    InputSystem InputSystem;

    PlayerV2 player(0.0f, 0.0f, 100.0f, 100.0f);
    player.active = true;
    addEntity(&player);
    int renderSize = sizeof(entityList) / sizeof(entityList[0]);

    // PlayerV2 player(0.0f, 0.0f, 100.0f, 100.0f);
    // player.active = true;
    // entityManager.addEntity(&player);

    // EntityV2* omo = entityManager.getEntity((char*)"player");
    // EntityV2* omo2 = entityManager.getEntity((char*)"bullet");

    // EntityV2* renderEntities[1] = {omo};
    // int renderSize = sizeof(renderEntities) / sizeof(renderEntities[0]);

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFaceDirection, camera.cameraUp);

    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glfwPollEvents();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        projection = glm::ortho(0.0f, (float)display_w, 0.0f, (float)display_h, -10.0f, 10.0f);
        view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFaceDirection, camera.cameraUp);

        // InputSystem.processInput(window);
        // InputSystem.listen(inputEntities, inputSize);

        testShader.use();
        // updateSystem.update(updateEntities, updateSize);
        renderSystem.render(entityList, renderSize, &testShader, projection, view);
        
        glfwSwapBuffers(window);
    }

    return 1;
}