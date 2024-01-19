#ifndef ENEMY_H
#define ENEMY_H

#include "entityV2.h"
#include "keyinput.h"
#include "systems/input_system.h"

class Enemy: public EntityV2 {
    public:
        glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

        float acceleration = 5.0f;

        Enemy() {};

        Enemy(Shader* entity_shader, float position_x, float position_y, float size_width, float size_height, int entity_index) {
            shader = entity_shader;

            x = position_x;
            y = position_y;
            width = size_width;
            height = size_height;
            index = entity_index;

            name = (char*)"enemy";

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
        }

        void performAction(char* action) override {
            if(action == (char*)"despawn") {
                printf("hi\n");
                despawn();
            }
        }

        void update() override {
            // stateChecker();
            // applySpeed();
            // applyPosition();
        }

        void render(Shader* shader, glm::mat4 projection, glm::mat4 view) override {
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
};

#endif