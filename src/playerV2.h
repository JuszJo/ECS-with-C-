#ifndef PLAYERV2_H
#define PLAYERV2_H

#include "entityV2.h"

class PlayerV2: public EntityV2 {
    public:
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

        void render(Shader* shader, glm::mat4 projection, glm::mat4 view) {
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
};

#endif