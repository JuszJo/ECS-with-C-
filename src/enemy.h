#ifndef ENEMY_H
#define ENEMY_H

#include <random>

#include "entityV2.h"
#include "keyinput.h"
#include "systems/input_system.h"

#include "bullet_manager.h"

class Enemy: public EntityV2 {
    public:
        glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

        float acceleration = 5.0f;

        int shootBuffer = 50;
        int elapsed = 0;

        std::random_device rd;
        int maxShootBuffer = 300;

        Enemy() {};

        Enemy(Shader* entity_shader, float position_x, float position_y, float size_width, float size_height) {
            shader = entity_shader;

            x = position_x;
            y = position_y;
            width = size_width;
            height = size_height;

            name = (char*)"enemy";

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

            loadImage((char*)"src\\assets\\enemyship.png", &TBO);
        }

        void applySpeed() {
            x += speed.x;
            y += speed.y;
        }

        void applyPosition() {
            model = glm::mat4(1.0f);

            model = glm::translate(model, glm::vec3(x, y, 0.0f));
        }

        void despawn() {
            active = false;
            --enemyCount;
        }

        void setMaxShootBuffer(int amount) {
            maxShootBuffer = amount;
        }

        void performAction(char* action) override {
            if(action == (char*)"despawn") {
                // printf("hi\n");
                despawn();
            }
            if(action == (char*)"reduce_max_shoot_buffer") {
                setMaxShootBuffer(maxShootBuffer - ((50 * currentLevel) - 50));
            }
        }

        int random(int min = 10, int max = 300) {
            std::mt19937 gen(rd());

            std::uniform_int_distribution<int> distribution(10, max);

            int randomNumber = distribution(gen);

            return randomNumber;
        }

        void checkShoot() {
            if(elapsed % shootBuffer == 0) {
                BulletManager::createBullet(shader, x + (width / 2) - (BulletManager::width / 2), y - BulletManager::height, "red", true);

                elapsed = 0;

                if(maxShootBuffer < 100) maxShootBuffer = 100;

                int check = random(10, maxShootBuffer);

                // std::cout << "random: " << check << std::endl;

                shootBuffer = check;
            }

            // std::cout << "elapsed shoot: " << elapsed << " ";

            ++elapsed;
        }

        void update() override {
            // stateChecker();
            checkShoot();
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
};

#endif