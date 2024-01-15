#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "libs/shader.h"
#include "libs/stb_image.h"

#include "src/entityV2.h"

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

int size = 1;

EntityV2** entityList = (EntityV2**)malloc(size * sizeof(EntityV2*));
int currentIndex = 0;

int activeBullets = 0;
int maxActiveBullets = 1;

// bool sizeChanged = false;

void addEntity(EntityV2* entity) {
    if(currentIndex == size) {
        // printf("UHMM\n");

        int newSize = size + 1;

        entityList = (EntityV2**)realloc(entityList, newSize * sizeof(EntityV2*));

        // Check if realloc succeeded
        if (entityList == NULL) {
            printf("Error: Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        size = newSize;
    }

    entityList[currentIndex] = entity;

    ++currentIndex;
}

void removeEntity(int entity_index) {
    delete entityList[entity_index];
    
    for(int i = entity_index; i < currentIndex - 1; ++i) {
        entityList[i] = entityList[i + 1];
    }

    --currentIndex;

    // sizeChanged = true;

    /* int newSize = size - 1;

    entityList = (EntityV2**)realloc(entityList, newSize * sizeof(EntityV2*));

    if (entityList == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    size = newSize;

    std::cout << "after removal: " << size << std::endl; */
}

void removeNotActive() {
    std::cout << "current size: " << size << std::endl;
    int notActiveSize = 0;

    for(int i = 0; i < size; ++i) {
        if(entityList[i] -> active == false) {
            ++notActiveSize;
        }
    }

    int newSize = size - notActiveSize;

    if(newSize != size) {
        // delete entityList[entity_index];
    
        // for(int i = entity_index; i < currentIndex - 1; ++i) {
        //     entityList[i] = entityList[i + 1];
        // }

        // --currentIndex;
        for(int i = 0; i < size; ++i) {
            if(entityList[i] -> active == false) {
                delete entityList[i];

                for(int j = i; j < size - 1; ++j) {
                    entityList[j] = entityList[j + 1];
                }

                --currentIndex;
            }
        }

        entityList = (EntityV2**)realloc(entityList, newSize * sizeof(EntityV2*));

        if (entityList == NULL) {
            printf("Error: Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        size = newSize;

        std::cout << "after removal: " << size << std::endl;
    }
}

#include "src/game_manager.h"
#include "src/entity_manager.h"

#include "src/systems/render_system.h"
#include "src/systems/update_system.h"
#include "src/systems/input_system.h"

#include "src/playerV2.h"
#include "src/bullet.h"

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

    UpdateSystem updateSystem;
    RenderSystem renderSystem;
    InputSystem InputSystem;

    PlayerV2 player(0.0f, 0.0f, 100.0f, 100.0f, currentIndex);
    player.active = true;
    addEntity(&player);

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

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            /* PlayerV2* newP = new PlayerV2(50.0f * (float)size, 0.0f, 100.0f, 100.0f);

            newP -> active = true;

            addEntity(newP); */
            if(activeBullets >= maxActiveBullets) {

            }
            else {
                Bullet* newBullet = new Bullet(0.0f, 0.0f, 20.0f, 20.0f, currentIndex);

                newBullet -> x = 300.0f;
                newBullet -> y = 300.0f;

                newBullet -> active = true;

                addEntity(newBullet);

                ++activeBullets;
            }
        }

        // std::cout << size << std::endl;

        InputSystem.processInput(window);
        InputSystem.listen(entityList, size);

        testShader.use();
        updateSystem.update(entityList, size);
        renderSystem.render(entityList, size, &testShader, projection, view);

        removeNotActive();
        
        glfwSwapBuffers(window);
    }

    return 1;
}