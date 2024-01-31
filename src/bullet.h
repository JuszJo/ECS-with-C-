#ifndef BULLET_H
#define BULLET_H

#include "entityV2.h"

class Bullet: public EntityV2 {
    public:
        glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

        float acceleration = 5.0f;

        float color[3];

        Bullet() {};

        Bullet(Shader* entity_shader, float position_x, float position_y, float size_width, float size_height) {
            shader = entity_shader;
            
            x = position_x;
            y = position_y;
            width = size_width;
            height = size_height;            

            name = (char*)"bullet";

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

            loadImage((char*)"src\\assets\\laser.png", &TBO);
        }

        void applySpeed() {
            x += speed.x;
            y += speed.y;
        }

        void applyPosition() {
            model = glm::mat4(1.0f);

            model = glm::translate(model, glm::vec3(x, y, 0.0f));
        }

        void removeBullet() {
            active = false;
            --activeBullets;
        }

        void performAction(char* action) override {
            if(action == (char*)"remove_bullet") {
                removeBullet();
            }
        }

        void update() override {
            speed.y = acceleration;
            applySpeed();
            applyPosition();

            if(x + width > 800.0f) {
                removeBullet();
            }
            if(y + height > 600.0f) {
                removeBullet();
            }
            if(y < 0.0f) {
                removeBullet();
            }

            // std::cout << "bullet isActive: " << active << std::endl;
        }

        void render(glm::mat4 projection, glm::mat4 view) override {
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            setUniformVec3f(shader, (char*)"bulletColor", color[0], color[1], color[2]);
            glBindTexture(GL_TEXTURE_2D, TBO);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
};

#endif