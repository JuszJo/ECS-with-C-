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

// TODO: Add Explosions

// WINDOW SETTINGS
int display_w, display_h;
double cursor_position_x, cursor_position_y;

// GAME SETTINGS
bool gameStart;
bool gameOver;
bool pause;
int gameOverBuffer;
int elapsedGameOverFrames;

// LEVEL SETTINGS
int currentLevel;
int currentWave;

// BULLETS SETTINGS
int activeBullets;
int maxActiveBullets;

// ENEMY SETTINGS
/* float enemyStartingPositionY;
int enemyCount;
int spawnBuffer;
int elapsedSpawnFrames; */

void pauseGame() {
    pause = true;
}

void resumeGame() {
    pause = false;
}

void initGameSettings() {
    // GAME SETTINGS
    gameStart = false;
    gameOver = false;
    pause = false;
    gameOverBuffer = 100;
    elapsedGameOverFrames = 1;

    // LEVEL SETTINGS
    currentLevel = 1;
    currentWave = 1;

    // BULLETS SETTINGS
    activeBullets = 0;
    maxActiveBullets = 30;

    // ENEMY SETTINGS
    // enemyStartingPositionY = 530.0f;
    // enemyCount = 0;
    // spawnBuffer = 100;
    // elapsedSpawnFrames = 1;
}

/* void gameActions(EntityV2* entity1, EntityV2* entity2) {
    if(entity1 -> name == (char*)"bullet" && entity2 -> name == (char*)"enemy") {
        entity1 -> performAction((char*)"remove_bullet");
        entity2 -> performAction((char*)"despawn");
        // addEvent(EVENTS::ENEMY_DESPAWN);
    }
    if(entity1 -> name == (char*)"bullet" && entity2 -> name == (char*)"player") {
        entity1 -> performAction((char*)"remove_bullet");
        entity2 -> performAction((char*)"despawn");
    }
} */

struct Camera {
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraFaceDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float cameraSpeed = 2.0f;
};

Camera camera;

EntityManager entityManager;

#include "src/playerV2.h"
#include "src/enemy.h"
#include "src/bullet.h"

#include "src/button.h"
#include "src/menu.h"

#include "src/bullet_manager.h"
#include "src/enemy_manager.h"

#include "src/systems/render_system.h"
#include "src/systems/update_system.h"
#include "src/systems/input_system.h"
#include "src/systems/collision_system.h"

void start_game() {
    gameStart = true;
}

void nextLevel() {
    ++currentLevel;
    ++currentWave;

    std::cout << "current Level: " << currentLevel << std::endl;
    
    EnemyManager::createMulitipleEnemies(0.0f, EnemyManager::enemyStartingPositionY, 10);
    EnemyManager::reduceMaxShootBuffer();

    EnemyManager::elapsedSpawnFrames = 0;
}

void resetGame() {
    gameStart = false;
    gameOver = false;

    elapsedGameOverFrames = 0;

    currentLevel = 1;
    currentWave = 1;

    // BULLETS SETTINGS
    activeBullets = 0;
    
    EnemyManager::resetSettings();
    // ENEMY SETTINGS
    // enemyCount = 0;
    // elapsedSpawnFrames = 1;
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

    initGameSettings();

    Shader playerShader("shaders/player/vertShader.glsl", "shaders/player/fragShader.glsl");
    Shader bulletShader("shaders/bullet/vertShader.glsl", "shaders/bullet/fragShader.glsl");
    Shader testShader("shaders/test/vertShader.glsl", "shaders/test/fragShader.glsl");
    Shader menuShader("shaders/menu/menuVertexShader.glsl", "shaders/menu/menuFragmentShader.glsl");

    BulletManager::bulletShader = &bulletShader;
    EnemyManager::shader = &playerShader;

    UpdateSystem updateSystem;
    RenderSystem renderSystem;
    InputSystem inputSystem;
    CollisionSystem collisionSystem;

    PlayerV2 player(&playerShader, 0.0f, 0.0f, 40.0f, 40.0f);
    player.active = true;
    player.setPosition(400.0f - player.width / 2, 10.0f);
    entityManager.addEntity(&player);

    EnemyManager::createMulitipleEnemies(0.0f, EnemyManager::enemyStartingPositionY, 10);

    Button playButton(&menuShader, (char*)"src\\assets\\play.png", 0.0f, 0.0f, 100.0f, 50.0f, start_game);
    playButton.updatePosition(400.0f - (playButton.width / 2), 300.0f - (playButton.height / 2));
    Menu mainMenu(&menuShader, (char*)"src\\assets\\mainmenu3.png", 0.0f, 0.0f, 800.0f, 600.0f);

    Button resumeButton(&menuShader, (char*)"src\\assets\\resume.png", 0.0f, 0.0f, 100.0f, 50.0f, resumeGame);
    resumeButton.updatePosition(400.0f - (resumeButton.width / 2), 300.0f - (resumeButton.height / 2));
    Menu playerMenu(&menuShader, (char*)"src\\assets\\mainmenu3.png", 0.0f, 0.0f, 800.0f, 600.0f);

    unsigned int TBO;

    int texture_width, texture_height, nChannels;

    stbi_uc* imageData = stbi_load((char*)"src\\assets\\bgmain2.png", &texture_width, &texture_height, &nChannels, 0);

    if(!imageData) {
        const char* reason = stbi_failure_reason();

        std::cout << reason << std::endl;
    }

    glGenTextures(1, &TBO);
    glBindTexture(GL_TEXTURE_2D, TBO);

    if(nChannels > 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imageData);

    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TBO, 0);

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
        
        glBlitFramebuffer(0, 0, 800.0f, 600.0f, 0, 0, 800.0f, 600.0f, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        projection = glm::ortho(0.0f, (float)display_w, 0.0f, (float)display_h, -10.0f, 10.0f);
        view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFaceDirection, camera.cameraUp);

        if(!gameStart) {
            mainMenu.render(projection);
            playButton.checkMousePress(window);
            playButton.update();
            playButton.render(projection);
        }
        else {
            if(pause) {
                playerMenu.render(projection);
                resumeButton.checkMousePress(window);
                resumeButton.update();
                resumeButton.render(projection);
            }
            else {
                inputSystem.processInput(window);
                inputSystem.listen();

                updateSystem.update();
                collisionSystem.wallCollision();
                collisionSystem.checkCollision();
                renderSystem.render(projection, view);
            }
        }

        if(EnemyManager::enemyCount == 0) {
            std::cout << "Enemy will spawn in: " << EnemyManager::spawnBuffer - EnemyManager::elapsedSpawnFrames << std::endl;

            if(EnemyManager::elapsedSpawnFrames % EnemyManager::spawnBuffer == 0) nextLevel();

            ++EnemyManager::elapsedSpawnFrames;
        }

        if(gameOver) {
            std::cout << "time to game closing: " << gameOverBuffer - elapsedGameOverFrames << std::endl;
            if(elapsedGameOverFrames % gameOverBuffer == 0) {
                resetGame();

                EnemyManager::removeAllEnemies();
                BulletManager::removeAllBullets();
                player.resetEntity();

                EnemyManager::createMulitipleEnemies(0.0f, EnemyManager::enemyStartingPositionY, 10);

                entityManager.addEntity(&player);
            }

            ++elapsedGameOverFrames;
        }

        entityManager.removeInactive();

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