#ifndef ENTITY_H
#define ENTITY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../libs/stb_image.h"

#include "../libs/shader.h"

class Entity {
    public:
        unsigned int VAO, VBO, EBO, TBO;
        glm::mat4 model = glm::mat4(1.0f);

        void setProjection(Shader* shader, glm::mat4 projection) {
            int location = glGetUniformLocation(shader -> shaderProgram, "projection");
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
        }

        void setUniformVec3f(Shader* shader, char* name, float val1, float val2, float val3) {
            int colorLocation = glGetUniformLocation(shader -> shaderProgram, name);
            glUniform3f(colorLocation, val1, val2, val3);
        }

        void setUniform1f(Shader* shader, char* name, float value) {
            int location = glGetUniformLocation(shader -> shaderProgram, name);
            glUniform1f(location, value);
        }

        void setUniform1i(Shader* shader, char* name, float value) {
            int location = glGetUniformLocation(shader -> shaderProgram, name);
            glUniform1f(location, value);
        }

        void setUniformMatrix4fv(Shader* shader, char* name) {
            int location = glGetUniformLocation(shader -> shaderProgram, name);
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
        }

    protected:
        void genVertexandBuffers(unsigned int* VAO, unsigned int* VBO) {
            glGenVertexArrays(1, VAO);
            glGenBuffers(1, VBO);
        }

        void genElementBuffers(unsigned int* EBO) {
            glGenBuffers(1, EBO);
        }

        void bindVAO(unsigned int VAO) {
            glBindVertexArray(VAO);
        }

        void handleVertexBufferObject(unsigned int VBO, float* vertices, float size) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        }

        void handleElementBuffer(unsigned int EBO, unsigned int* indices, float size) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_DYNAMIC_DRAW);
        }

        void handleVertexArrayObject(int location, int nComponents, int stride, int offset) {
            glVertexAttribPointer(location, nComponents, GL_FLOAT, GL_FALSE, stride * sizeof(float), offset == 0 ? nullptr : (void*)(offset * sizeof(float)));
            glEnableVertexAttribArray(location);
        }

        void loadImage(char* path) {
            int width, height, nChannels;

            stbi_uc* imageData = stbi_load(path, &width, &height, &nChannels, 0);

            if(!imageData) {
                const char* reason = stbi_failure_reason();

                std::cout << reason << std::endl;
            }

            glGenTextures(1, &TBO);
            glBindTexture(GL_TEXTURE_2D, TBO);

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

        void cleanupBuffers() {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
};

#endif