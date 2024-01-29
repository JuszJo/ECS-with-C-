#ifndef PLAYERV2_H
#define PLAYERV2_H

#include "entityV2.h"
#include "keyinput.h"
#include "systems/input_system.h"

#include "bullet_manager.h"

class PlayerV2: public EntityV2 {
    public:
        glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

        float acceleration = 5.0f;

        enum STATE {UP, DOWN, LEFT, RIGHT, IDLE};

        STATE currentState = IDLE;

        int shootBuffer = 25;
        int elapsed = 1;

        bool canShoot = true;

        PlayerV2() {};

        PlayerV2(Shader* entity_shader, float position_x, float position_y, float size_width, float size_height) {
            shader = entity_shader;
            
            x = position_x;
            y = position_y;
            width = size_width;
            height = size_height;

            name = (char*)"player";

            stride = 5;

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

            loadImage((char*)"src\\assets\\spaceship.png", &TBO);
        }

        void listen(KeyInput::Keys* keys) override {
            if(keys -> w) {
                // printf("w\n");
                // currentState = UP;
            }
            if(keys -> s) {
                // printf("s\n");
                // currentState = UP;
            }
            if(keys -> a) {
                // printf("a\n");
                currentState = LEFT;
            }
            if(keys -> d) {
                // printf("d\n");
                currentState = RIGHT;
            }
            if(keys -> p || keys -> esc) {
                pauseGame();
            }
            if(keys -> space) {
                if(canShoot) {
                    BulletManager::createBullet(shader, (x + (width / 2)) - (BulletManager::width / 2), y + height + 2.0f, "blue");

                    canShoot = false;
                }
            }
            if(!keys -> w && !keys -> s && !keys -> a && !keys -> d) {
                currentState = IDLE;
            }
        }

        void shootCooldown() {
            if(elapsed % shootBuffer == 0) {
                canShoot = true;

                elapsed = 0;
            }

            ++elapsed;
        }

        void stateChecker() {
            switch (currentState) {
                case LEFT:
                    speed.x = -acceleration;
                    
                    break;

                case RIGHT:
                    speed.x = acceleration;

                    break;

                case IDLE:
                    speed.x = 0.0f;

                    break;
                
                default:
                    break;
            }
        }

        void despawn() {
            active = false;
            gameOver = true;
        }

        void performAction(char* action) {
            if(action == (char*)"despawn") {
                printf("player died\n");
                despawn();
            }
        }

        void applySpeed() {
            x += speed.x;
            y += speed.y;
        }

        void applyPosition() {
            model = glm::mat4(1.0f);

            model = glm::translate(model, glm::vec3(x, y, 0.0f));
        }

        void update() override {
            if(!canShoot) shootCooldown();
            stateChecker();
            applySpeed();
            applyPosition();
        }

        void render(glm::mat4 projection, glm::mat4 view) override {
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindTexture(GL_TEXTURE_2D, TBO);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        void resetEntity() {
            resetModel();
            setPosition(400.0f - width / 2, 10.0f);

            active = true;
            
            speed = glm::vec3(0.0f, 0.0f, 0.0f);
            acceleration = 5.0f;

            currentState = IDLE;

            shootBuffer = 25;
            elapsed = 1;

            canShoot = true;
        }
};

#endif