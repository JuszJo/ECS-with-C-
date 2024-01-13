#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "libs/shader.h"
#include "libs/stb_image.h"

// #include "src/square.h"
// #include "src/button.h"
// #include "src/menu.h"

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

#include "src/systems/render_system.h"
#include "src/entityManager.h"

#include "src/entityV2.h"
#include "src/playerV2.h"

Camera camera;


void test(EntityV2* pointer) {
    std::cout << pointer->active << std::endl;
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

    RenderSystem renderSystem;

    PlayerV2 player(0.0f, 0.0f, 100.0f, 100.0f);
    player.active = true;
    entityManager.addEntity(&player);

    EntityV2* omo = entityManager.getEntity((char*)"player");

    EntityV2** renderEntities = {&omo};

    int renderSize = sizeof(renderEntities) / sizeof(renderEntities[0]);

    // EntityV2* arr[1];

    // arr[0] = &player;

    // int size = sizeof(arr) / sizeof(arr[0]);

    // std::cout << entityManager.getEntity((char*)"player") -> name << std::endl;

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

        testShader.use();
        renderSystem.render(renderEntities, renderSize, &testShader, projection, view);
        
        glfwSwapBuffers(window);
    }

    return 1;
}