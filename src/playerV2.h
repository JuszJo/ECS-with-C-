#ifndef PLAYERV2_H
#define PLAYERV2_H

#include "entityV2.h"
#include "keyinput.h"
#include "systems/input_system.h"

class PlayerV2: public EntityV2 {
    public:
        glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

        float acceleration = 5.0f;

        enum STATE {UP, DOWN, LEFT, RIGHT, IDLE};

        STATE currentState = IDLE;

        PlayerV2() {};

        PlayerV2(float position_x, float position_y, float size_width, float size_height) {
            x = position_x;
            y = position_y;
            width = size_width;
            height = size_height;

            name = (char*)"player";

            stride = 3;

            float vertices[20] = {
                x, y, 0.0f,
                x + width, y, 0.0f,
                x, y + height, 0.0f,
                x + width, y + height, 0.0f
            };

            genVertexandBuffers(&VAO, &VBO);
            bindVAO(VAO);

            int verticeSize = sizeof(vertices);
            handleVertexBufferObject(VBO, vertices, verticeSize);

            handleVertexArrayObject(0, 3, stride, 0);
            // handleVertexArrayObject(1, 2, stride, 3);

            cleanupBuffers();
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
            if(!keys -> w && !keys -> s && !keys -> a && !keys -> d) {
                currentState = IDLE;
            }
        }

        void move() {

        }

        void render(Shader* shader, glm::mat4 projection, glm::mat4 view) {
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
};

#endif