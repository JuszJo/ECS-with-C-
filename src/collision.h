#ifndef COLLISION_H
#define COLLISION_H

#include <iostream>
#include "../libs/shader.h"

struct CollidableBlock {
    float position_x;
    float position_y;
    float width;
    float height;
};

class Collision {
    public:
        unsigned int VAO, VBO;

        glm::mat4 model = glm::mat4(1.0f);

        CollidableBlock block1 = {0.0f, 0.0f, 800.0f, 140.0f};
        // CollidableBlock block2 = {100.0f, 100.0f, 100.0f, 100.0f};
        // CollidableBlock block3 = {200.0f, 100.0f, 100.0f, 100.0f};

        CollidableBlock blocks[1] = {
            block1
            // block2
            // block3
        };

        float x = block1.position_x;
        float y = block1.position_y;
        float width = block1.width;
        float height = block1.height;

        Collision() {
            // position_x = x;
            // position_y = y;
            // bgWidth = width;
            // bgHeight = height;

            float vertices[] = {
                x, y, 0.0f, 0.9f, 0.3f, 0.3f, 1.0f,
                x + width, y, 0.0f, 0.9f, 0.3f, 0.3f, 1.0f,
                x, y + height, 0.0f, 0.9f, 0.3f, 0.3f, 1.0f,
                x + width, y + height, 0.0f, 0.9f, 0.3f, 0.3f, 1.0f
            };

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        char* getCollideAxisX(float left, float right) {
            return left < right ? (char*)"left" : (char*)"right";
        }

        char* getCollideAxisY(float bottom, float top) {
            return bottom < top ? (char*)"bottom" : (char*)"top";
        }

        bool didCollide(float position_x, float position_y, float width, float height, CollidableBlock currentBlock) {
            if(
                position_x < currentBlock.position_x + currentBlock.width &&
                position_x + width > currentBlock.position_x &&
                position_y < currentBlock.position_y + currentBlock.height &&
                position_y + height > currentBlock.position_y
            ) {
                return true;
            }
            else return false;
        }

        bool didCollideBest(float position_x, float position_y, float width, float height, float nextPosition_x, float nextPosition_y, float nextWidth, float nextHeight) {
            if(
                position_x < nextPosition_x + nextWidth &&
                position_x + width > nextPosition_x &&
                position_y < nextPosition_y + nextHeight &&
                position_y + height > nextPosition_y
            ) {
                return true;
            }
            else return false;
        }

        // CollisionInfo checkCollision(float position_x, float position_y, float width, float height) {
        //     bool collisionOccured = false;

        //     for(int i = 0; i < sizeof(blocks) / sizeof(blocks[0]); ++i) {
        //         CollidableBlock currentBlock = blocks[i];

        //         if(didCollide(position_x, position_y, width, height, currentBlock)) {
        //             collisionOccured = true;

        //             CollisionInfo info = {i};

        //             return info;
        //         }
        //     }

        //     CollisionInfo info = {-1};

        //     return info;
        // }

        void render(Shader* shader, glm::mat4 projection, glm::mat4 view) {
            if(!settingShowCollisionbox) return;
            for(CollidableBlock block : blocks) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(block.position_x, block.position_y, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
                glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
        }
};

#endif