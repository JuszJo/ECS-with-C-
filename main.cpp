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

// ENTITY GLOBALS
int size = 1;

EntityV2** entityList = (EntityV2**)malloc(size * sizeof(EntityV2*));
int currentIndex = 0;

// EVENT GLOBALS
enum EVENTS {
    NONE,
    ENEMY_DESPAWN
};

int eventSize = 1;
int realEventSize = 0;
EVENTS* eventList = (EVENTS*)malloc(eventSize * sizeof(EVENTS));
int currentEventIndex = 0;

// EVENT GLOBALS FUNCTIONS
void addEvent(EVENTS event) {
    if(currentEventIndex == eventSize) {
        // printf("UHMM\n");

        int newSize = eventSize + 1;

        eventList = (EVENTS*)realloc(eventList, newSize * sizeof(EVENTS));

        // Check if realloc succeeded
        if (eventList == NULL) {
            printf("Error: Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        eventSize = newSize;
    }

    eventList[currentEventIndex] = event;

    ++currentEventIndex;

    ++realEventSize;
}

void testEvents() {
    for(int i = 0; i < realEventSize; ++i) {
        if(eventList[i] == EVENTS::ENEMY_DESPAWN) {
            std::cout << "Event " << i + 1 << ": Enemy Despawn" << std::endl;
        }
        if(eventList[i] == EVENTS::NONE) {
            std::cout << "Event " << i + 1 << ": None" << std::endl;
        }
    }
}

// BULLETS SETTINGS
int activeBullets = 0;
int maxActiveBullets = 1;

// ENEMY SETTINGS
int spawnBuffer = 100;
int elapsedSpawnFrames = 0;

// ENTITY GLOBALS FUNCTIONS
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

bool didCollide(EntityV2 entity1, EntityV2 entity2) {
    if(
        entity1.x + entity1.width > entity2.x &&
        entity1.x < entity2.x + entity2.width &&
        entity1.y + entity1.height > entity2.y &&
        entity1.y < entity2.y + entity2.height
    ) {
        return true;
    }
    else {
        return false;
    }
}

void gameActions(EntityV2* entity1, EntityV2* entity2) {
    if(entity1 -> name == (char*)"bullet" && entity2 -> name == (char*)"enemy") {
        entity1 -> performAction((char*)"remove_bullet");
        entity2 -> performAction((char*)"despawn");
        addEvent(EVENTS::ENEMY_DESPAWN);
    }
}

void entityCollision(EntityV2* entity) {
    for(int i = 0; i < size; ++i) {
        EntityV2* currentEntity = entityList[i];
        if(currentEntity != entity) {
            if(didCollide(*entity, *currentEntity)) {
                gameActions(entity, currentEntity);
                // std::cout << "collided with" << currentEntity -> name << std::endl;
            }
        }
    }
}

void GameListener() {

}

#include "src/systems/render_system.h"
#include "src/systems/update_system.h"
#include "src/systems/input_system.h"
#include "src/systems/collision_system.h"

#include "src/playerV2.h"
#include "src/enemy.h"
#include "src/bullet.h"

#include "src/bullet_manager.h"
#include "src/enemy_manager.h"

Camera camera;


void test(EntityV2* pointer) {
    std::cout << pointer->active << std::endl;
}

// TODO: MAKE MORE FIXES
// TODO: ADD SPAWNING ENEMIES

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

    UpdateSystem updateSystem;
    RenderSystem renderSystem;
    InputSystem inputSystem;
    CollisionSystem collisionSystem;

    PlayerV2* player = new PlayerV2(&testShader, 0.0f, 0.0f, 100.0f, 100.0f, currentIndex);
    player -> active = true;
    addEntity(player);

    Enemy* enemy = new Enemy(&testShader, 600.0f, 300.0f, 100.0f, 100.0f, currentIndex);
    enemy -> active = true;
    addEntity(enemy);

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

        /* if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            BulletManager::createBullet(&testShader, 300.0f, 300.0f);
        } */

        // std::cout << size << std::endl;

        inputSystem.processInput(window);
        inputSystem.listen();

        // testShader.use();
        updateSystem.update();
        collisionSystem.checkCollision();
        renderSystem.render(&testShader, projection, view);

        /* if(activeBullets) {
            bool result = entityCollision(*entityList[2], enemy);

            std::cout << "did collide?: " << result << std::endl;
        } */

        removeNotActive();
        testEvents();
        
        glfwSwapBuffers(window);
    }

    return 1;
}