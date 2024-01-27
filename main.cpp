#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "libs/shader.h"
#include "libs/stb_image.h"
#include "utils/my_array.h"

#include "src/entityV2.h"
#include "src/entity_manager.h"

// EntityManager entityManager;

// TODO: Add random enemy random shot

struct Camera {
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraFaceDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float cameraSpeed = 2.0f;
};

// WINDOW SETTINGS
int display_w, display_h;
double cursor_position_x, cursor_position_y;

// GAME SETTINGS
bool gameStart = false;
bool gameOver = false;
int gameOverBuffer = 100;
int elapsedGameOverFrames = 1;

// LEVEL SETTINGS
int currentLevel = 1;
int currentWave = 1;

// BULLETS SETTINGS
int activeBullets = 0;
int maxActiveBullets = 30;

// ENEMY SETTINGS
float enemyStartingPositionY = 500.0f;
int enemyCount = 0;
int spawnBuffer = 100;
int elapsedSpawnFrames = 1;

void startAgain() {
    gameStart = false;
    gameOver = false;
    gameOverBuffer = 100;
    elapsedGameOverFrames = 1;

    // LEVEL SETTINGS
    currentLevel = 1;
    currentWave = 1;

    // BULLETS SETTINGS
    activeBullets = 0;
    maxActiveBullets = 30;

    // ENEMY SETTINGS
    enemyStartingPositionY = 500.0f;
    enemyCount = 0;
    spawnBuffer = 100;
    elapsedSpawnFrames = 1;
}

void gameActions(EntityV2* entity1, EntityV2* entity2) {
    if(entity1 -> name == (char*)"bullet" && entity2 -> name == (char*)"enemy") {
        entity1 -> performAction((char*)"remove_bullet");
        entity2 -> performAction((char*)"despawn");
        // addEvent(EVENTS::ENEMY_DESPAWN);
    }
    if(entity1 -> name == (char*)"bullet" && entity2 -> name == (char*)"player") {
        entity1 -> performAction((char*)"remove_bullet");
        entity2 -> performAction((char*)"despawn");
    }
}

EntityManager entityManager;

#include "src/systems/render_system.h"
#include "src/systems/update_system.h"
#include "src/systems/input_system.h"
#include "src/systems/collision_system.h"

#include "src/playerV2.h"
#include "src/enemy.h"
#include "src/bullet.h"

#include "src/button.h"
#include "src/menu.h"

#include "src/bullet_manager.h"
#include "src/enemy_manager.h"

Camera camera;


void test(EntityV2* pointer) {
    std::cout << pointer->active << std::endl;
}

void start_game() {
    gameStart = true;
}

int main() {
    if (!glfwInit()) return 1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Space Shooters", NULL, NULL);

    if (!window) {
        glfwTerminate();

        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glfwGetCursorPos(window, &cursor_position_x, &cursor_position_y);

    glewInit();

    // Shader shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    // Shader bgShader("shaders/background/bgVertexShader.glsl", "shaders/background/bgFragmentShader.glsl");
    // Shader cShader("shaders/collision/collisionVertexShader.glsl", "shaders/collision/collisionFragmentShader.glsl");
    // Shader menuShader("shaders/menu/menuVertexShader.glsl", "shaders/menu/menuFragmentShader.glsl");

    Shader playerShader("shaders/player/vertShader.glsl", "shaders/player/fragShader.glsl");
    Shader bulletShader("shaders/bullet/vertShader.glsl", "shaders/bullet/fragShader.glsl");
    Shader testShader("shaders/test/vertShader.glsl", "shaders/test/fragShader.glsl");
    Shader menuShader("shaders/menu/menuVertexShader.glsl", "shaders/menu/menuFragmentShader.glsl");

    BulletManager::bulletShader = &bulletShader;

    UpdateSystem updateSystem;
    RenderSystem renderSystem;
    InputSystem inputSystem;
    CollisionSystem collisionSystem;

    /* PlayerV2* player = new PlayerV2(&testShader, 0.0f, 0.0f, 40.0f, 40.0f, currentIndex);
    player -> active = true;
    addEntity(player); */
    PlayerV2 player(&playerShader, 0.0f, 0.0f, 40.0f, 40.0f);
    player.active = true;
    entityManager.addEntity(&player);

    EnemyManager::createMulitipleEnemies(&playerShader, 0.0f, enemyStartingPositionY, 10);

    Button playButton(&menuShader, (char*)"src\\assets\\play.png", 0.0f, 0.0f, 100.0f, 50.0f, start_game);
    playButton.updatePosition(400.0f - (playButton.width / 2), 300.0f - (playButton.height / 2));

    Menu mainMenu(&menuShader, (char*)"src\\assets\\mainmenu3.png", 0.0f, 0.0f, 800.0f, 600.0f);

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
        if(!gameStart) {
            mainMenu.render(projection);
            playButton.checkMousePress(window);
            playButton.update();
            playButton.render(projection);
        }
        else {
            inputSystem.processInput(window);
            inputSystem.listen();

            updateSystem.update();
            collisionSystem.wallCollision();
            collisionSystem.checkCollision();
            renderSystem.render(projection, view);
        }

        entityManager.removeInactive();

        if(enemyCount == 0) {
            // std::cout << "Enemy Count: " << enemyCount << std::endl;
            std::cout << "Enemy will spawn in: " << spawnBuffer - elapsedSpawnFrames << std::endl;
            // std::cout << "spawn elapsed frames: " << elapsedSpawnFrames << std::endl;
            if(elapsedSpawnFrames % spawnBuffer == 0) {
                ++currentLevel;
                ++currentWave;

                std::cout << "current Level: " << currentLevel << std::endl;
                
                EnemyManager::createMulitipleEnemies(&playerShader, 0.0f, enemyStartingPositionY, 10);
                EnemyManager::reduceMaxShootBuffer();

                elapsedSpawnFrames = 0;
            }

            ++elapsedSpawnFrames;
        }

        if(gameOver) {
            std::cout << "time to game closing: " << gameOverBuffer - elapsedGameOverFrames << std::endl;
            if(elapsedGameOverFrames % gameOverBuffer == 0) {
                glfwSetWindowShouldClose(window, GL_TRUE);

                elapsedGameOverFrames = 1;
            }

            ++elapsedGameOverFrames;
        }

        // std::cout << "active bullets: " << activeBullets << std::endl;
        // testEvents();

        // std::cout << "Enemy Count: " << enemyCount << std::endl;

        glfwGetCursorPos(window, &cursor_position_x, &cursor_position_y);
        
        glfwSwapBuffers(window);
    }

    glDeleteProgram(testShader.shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 1;
}