#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "animation.h"
#include "gravity.h"
#include "collision.h"
#include "hitbox.h"

struct CollisionInfo {
    char* side;
    float overlap;
};

class Enemy: public Entity {
    private:
        int stride = 5;

    public:
        bool active = false;

        unsigned int TBO2, TBO3, TBO4, TBO5, TBO6;

        float enemyWidth, enemyHeight, enemyX, enemyY;

        Camera* camera = nullptr;

        const char* axis = "origin";

        Animation animation;

        glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

        float acceleration = 2.0f;

        bool canJump = false;

        enum STATE {
            UP,
            DOWN,
            LEFT,
            RIGHT,
            IDLE
        };

        enum ALTSTATE {
            ATTACK,
            ATTACKED,
            NONE
        };

        STATE currentState = IDLE;
        ALTSTATE currentAltState = NONE;

        Gravity gravity;

        Collision collision;

        Hitbox hitbox;
        Hitbox attackHitbox;

        // default constructor
        Enemy() {}

        Enemy(char* texturePath, float x, float y, float width, float height) {
            enemyX = x;
            enemyY = y;
            enemyWidth = width;
            enemyHeight = height;

            float vertices[20] = {
                x, y, 0.0f, 0.0f, 1.0f,
                x + width, y, 0.0f, 1.0f, 1.0f,
                x, y + height, 0.0f, 0.0f, 0.0f,
                x + width, y + height, 0.0f, 1.0f, 0.0f
            };
            genVertexandBuffers(&VAO, &VBO);
            bindVAO(VAO);

            int verticeSize = sizeof(vertices);
            handleVertexBufferObject(VBO, vertices, verticeSize);

            handleVertexArrayObject(0, 3, stride, 0);
            handleVertexArrayObject(1, 2, stride, 3);

            cleanupBuffers();

            loadImage(texturePath, &TBO);
            loadImage((char*)"src\\assets\\playeridle.png", &TBO2);
            loadImage((char*)"src\\assets\\player2.png", &TBO3);
            loadImage((char*)"src\\assets\\playerup.png", &TBO4);
            loadImage((char*)"src\\assets\\playerattack.png", &TBO5);
            loadImage((char*)"src\\assets\\playerattack2.png", &TBO6);

            Animation newAnimation((char*)"idle", 11.0f, 4, 1, nullptr, false);

            newAnimation.updateTextureBuffer(&TBO2);

            animation = newAnimation;

            Hitbox newHitbox(x, y, 30.0f, height);

            newHitbox.defineOffset(18.0f, 10.0f);

            hitbox = newHitbox;

            Hitbox newAttackHitbox(x, y, 50.0f, 40.0f);

            newAttackHitbox.defineOffset(18.0f, 10.0f);

            // attackHitbox = newAttackHitbox;
        }

        void loadImage(char* path, unsigned int* TBO) {
            int width, height, nChannels;

            stbi_uc* imageData = stbi_load(path, &width, &height, &nChannels, 0);

            if(!imageData) {
                const char* reason = stbi_failure_reason();

                std::cout << reason << std::endl;
            }

            glGenTextures(1, TBO);
            glBindTexture(GL_TEXTURE_2D, *TBO);

            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            if(nChannels > 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
            }
            else {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
            }

            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(imageData);
        }

        void processInput(GLFWwindow* window) {
            // if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            //     glfwSetWindowShouldClose(window, true);
            // }
            // if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            //     // if(currentState != UP) currentState = UP;
            // }
            if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
                // if(currentState != UP) currentState = UP;
                if(currentAltState != ATTACK) currentAltState = ATTACK;
            }
            else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                // speed.y = 10.0f;
                jump();
            }
            else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                if(currentState != LEFT) currentState = LEFT;
            }
            else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                if(currentState != RIGHT) currentState = RIGHT;
            }
            else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                // if(currentState != DOWN) currentState = DOWN;
                // resetModel();
                // setPosition(400.0f, 300.0f);
            }
            else if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
                // if(currentState != DOWN) currentState = DOWN;
                // resetModel();
                // model = glm::scale(model, glm::vec3(2.0f, 2.0f, 1.0f));
            }
            else if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
                // if(currentState != DOWN) currentState = DOWN;
                // resetModel();
                // resetPlayer();
            }
            else {
                currentState = IDLE;
            }
        }

        void jump() {
            if(canJump) {
                speed.y = 10.0f;
                canJump = false;
            }
        }

        void checkState() {
            switch (currentState) {
                case LEFT:
                    if(currentAltState == NONE) animation.setCurrentAnimation((char*)"left", 8.0f, 4, 2, &TBO3, true);
                    speed = glm::vec3(-acceleration, speed.y, 0.0f);
                    animation.shouldAnimate = true;
                    
                    break;

                case RIGHT:
                    if(currentAltState == NONE) animation.setCurrentAnimation((char*)"right", 8.0f, 4, 0, &TBO, false);
                    speed = glm::vec3(acceleration, speed.y, 0.0f);
                    animation.shouldAnimate = true;
                    // camera -> cameraPos += glm::normalize(glm::cross(camera -> cameraFaceDirection, camera -> cameraUp)) * camera -> cameraSpeed;
                    
                    break;

                case DOWN:
                    // animation.setCurrentAnimation((char*)"right", 8.0f, 4, 0, &TBO, false);
                    speed = glm::vec3(0.0f, -acceleration, 0.0f);
                    // animation.shouldAnimate = true;
                    
                    break;

                case UP:
                    // animation.setCurrentAnimation((char*)"right", 8.0f, 4, 0, &TBO, false);
                    speed = glm::vec3(0.0f, acceleration, 0.0f);
                    // animation.shouldAnimate = true;
                    
                    break;

                case IDLE:
                    if(currentAltState == NONE) animation.setCurrentAnimation((char*)"idle", 11.0f, 4, 1, &TBO2, false);
                    // speed = glm::vec3(0.0f, 0.0f, 0.0f);
                    speed = glm::vec3(0.0f, speed.y, 0.0f);
                    animation.shouldAnimate = true;
                    
                    break;
                
                default:
                    break;
            }
        }

        void checkAltState() {
            unsigned int* texture = currentState == LEFT ? &TBO6 : &TBO5;
            bool reversed = currentState == LEFT ? true : false;
            switch (currentAltState) {
                case ATTACK:    
                    animation.setCurrentAnimation((char*)"attack", 3.0f, 4, 3, texture, reversed);
                    // speed = glm::vec3(-acceleration, speed.y, 0.0f);
                    animation.shouldAnimate = true;

                    checkEnemyCollision(attackHitbox.position_x, attackHitbox.position_y, attackHitbox.width, attackHitbox.height);
                    // std::cout << "ATTACKING\n";
                    // std::cout << animation.currentFrame << std::endl;
                    if(animation.currentFrame == (currentState == LEFT ? 1 : 3)) {
                        currentAltState = NONE;
                    }
                    
                    break;

                case ATTACKED:
                    // speed.y = 5.0f;
                    setPosition(enemyX + 5.0f, enemyY + 5.0f);
                    currentAltState = NONE;

                default:
                    
                    break;
            }
        }

        void move() {
            model = glm::translate(model, speed);
            enemyX += speed.x;
            enemyY += speed.y;

            // printf("x: %f, y: %f\n", enemyX, enemyY);
        }

        void updatePosition(float newEnemyX, float newEnemyY) {
            enemyX = newEnemyX;
            enemyY = newEnemyY;
        }

        void updateSize(float newEnemyWidth, float newEnemyHeight) {
            enemyWidth = newEnemyWidth;
            enemyHeight = newEnemyHeight;
        }

        void setPosition(float newEnemyX, float newEnemyY) {
            model = glm::translate(model, glm::vec3(newEnemyX, newEnemyY, 0.0f));

            model = glm::translate(model, glm::vec3(-enemyX, -enemyY, 0.0f));

            updatePosition(newEnemyX, newEnemyY);
        }

        void scale(float scaleFactorX, float scaleFactorY) {
            
        }

        void checkEnemyCollision(float position_x, float position_y, float width, float height) {
            
        }

        void checkCollision(float position_x, float position_y, float width, float height) {
            for(int i = 0; i < sizeof(collision.blocks) / sizeof(collision.blocks[0]); ++i) {
                CollidableBlock currentBlock = collision.blocks[i];

                if(collision.didCollide(position_x, position_y, width, height, currentBlock)) {
                    // collisionResponse(currentBlock, hitbox.position_x, hitbox.position_y, hitbox.width, hitbox.height);
                    // collisionResponse(currentBlock, enemyX, enemyY, enemyWidth, enemyHeight);
                    collisionResponse(currentBlock, position_x, position_y, width, height);
                }
            }
        }

        void checkWallCollision() {
            if(hitbox.position_x < 0) {
                setPosition(0.0f - hitbox.x_offset, enemyY);
                hitbox.updateAxis(enemyX, enemyY);
            }
            if(hitbox.position_x + hitbox.width > 800.0f) {
                setPosition(800.0f - hitbox.width - hitbox.x_offset, enemyY);
                hitbox.updateAxis(enemyX, enemyY);
            }
        }

        void collisionResponse(CollidableBlock currentBlock, float x, float y, float width, float height) {
            // referse to enemy bottom and top and left and right
            float bottom = (float)abs(y - (currentBlock.position_y + currentBlock.height));
            float top = (float)abs((y + height) - currentBlock.position_y);

            float left = (float)abs(x - (currentBlock.position_x + currentBlock.width));
            float right = (float)abs((x + width) - currentBlock.position_x);

            // std::cout << bottom << "\t" << top << std::endl;
            // std::cout << collision.getCollideAxisY(bottom, top) << std::endl;
            // std::cout << left << "\t" << right << std::endl;
            // std::cout << collision.getCollideAxisX(left, right) << std::endl;

            CollisionInfo xInfo = {collision.getCollideAxisX(left, right), collision.getCollideAxisX(left, right) == (char*)"left" ? left : right};
            CollisionInfo yInfo = {collision.getCollideAxisY(bottom, top), collision.getCollideAxisY(bottom, top) == (char*)"bottom" ? bottom : top};

            CollisionInfo final = {xInfo.overlap < yInfo.overlap ? xInfo.side : yInfo.side, xInfo.overlap < yInfo.overlap ? xInfo.overlap : yInfo.overlap};

            // printf("%s: %f\n", final.side, final.overlap);

            if(final.side == (char*)"bottom") {
                speed.y = 0.0f;

                canJump = true;

                setPosition(this -> enemyX, currentBlock.position_y + currentBlock.height - hitbox.y_offset);
                hitbox.updateAxis(this -> enemyX, currentBlock.position_y + currentBlock.height);
            }
            if(final.side == (char*)"right") {
                speed.x = 0.0f;

                setPosition(currentBlock.position_x - width - hitbox.x_offset, this -> enemyY);
                hitbox.updateAxis(currentBlock.position_x - width, this -> enemyY);
            }
        }

        void render(Shader* shader, glm::mat4 projection, glm::mat4 view) {
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            setUniform1f(shader, (char*)"totalFrames", animation.currentAnimatedState[0].totalFrames);
            setUniform1f(shader, (char*)"currentFrame", animation.currentFrame);
            glBindTexture(GL_TEXTURE_2D, *animation.currentAnimatedState[0].TBO);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            // resetModel();
        }

        void update() {
            gravity.applyGravity(&speed);
            checkState();
            checkAltState();
            move();
            animation.animate();
            hitbox.updateAxis(enemyX, enemyY);
            attackHitbox.updateAxis(enemyX, enemyY);
            // checkCollision(enemyX, enemyY, enemyWidth, enemyHeight);
            checkWallCollision();
            checkCollision(hitbox.position_x, hitbox.position_y, hitbox.width, hitbox.height);
            hitbox.updateAxis(enemyX, enemyY);
            attackHitbox.updateAxis(enemyX, enemyY);
        }

        void resetModel() {
            model = glm::mat4(1.0f);
        }

        void resetenemy() {
            setPosition(0.0f, 500.0f);
            
            hitbox.updateAxis(0.0f, 500.0f);

            speed = glm::vec3(0.0f, 0.0f, 0.0f);

            model = glm::mat4(1.0f);

            currentState = IDLE;
            currentAltState = NONE;

            canJump = false;
        }
};

#endif