#ifndef ENEMYFACTORY_H
#define ENEMYFACTORY_H

#include "entityV2.h"
#include "gravity.h"
#include "collision.h"
#include "hitbox.h"
#include "animation.h"

class EnemyFactory: public EntityV2 {
    public:
        struct EnemyData {
            bool active = false;
            float x;
            float y;
            glm::mat4 model = glm::mat4(1.0f);
            glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);
            Hitbox hitbox;
            Animation animation;
            // unsigned int TBO;
        };

        EnemyData factoryEnemies[100];
        int currentEnemyIndex = 0;

        int spawnBuffer = 200;
        int elapsedFrames = 0;

        Gravity gravity;

        Collision collision;

        struct CollisionInfo {
            char* side;
            float overlap;
        };

        float sinNumber = 90.0f;

        unsigned int runTBO;

        EnemyFactory(char* texturePath, float position_x, float position_y, float size_width, float size_height) {
            x = position_x;
            y = position_y;
            width = size_width;
            height = size_height;

            stride = 5;

            float vertices[] = {
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
            loadImage((char*)"src/assets/enemyrun.png", &runTBO);

            // EnemyData enemy;

            // enemy.x = 0.0f;
            // enemy.y = 0.0f;

            // Hitbox newHitbox(x, y, 30.0f, height);
            // newHitbox.defineOffset(18.0f, 10.0f);
            // enemy.hitbox = newHitbox;

            // Animation newAnimation((char*)"idle", 11.0f, 4, 1, &TBO, false);

            // enemy.animation = newAnimation;

            // enemy.active = false;

            // factoryEnemies[0] = enemy;
        }

        void addEnemy(float position_x, float position_y) {
            if(currentEnemyIndex > 99) {
                return;
            }
            EnemyData enemy;

            enemy.x = position_x;
            enemy.y = position_y;

            Hitbox newHitbox(position_x, position_y, 40.0f, 30.0f);
            newHitbox.defineOffset(18.0f, 0.0f);
            enemy.hitbox = newHitbox;

            Animation newAnimation((char*)"idle", 11.0f, 4, 1, &TBO, false);
            // Animation newAnimation((char*)"run", 6.0f, 4, 1, &runTBO, false);

            enemy.animation = newAnimation;

            enemy.active = true;

            factoryEnemies[currentEnemyIndex] = enemy;

            ++currentEnemyIndex;
        }

        // void setPosition(float newX, float newY) override {
        //     model = glm::translate(model, glm::vec3(newX, newY, 0.0f));

        //     model = glm::translate(model, glm::vec3(-x, -y, 0.0f));

        //     updatePosition(newX, newY);
        // }

        void checkCollision(float position_x, float position_y, float width, float height, EnemyData* currentEnemy) {
            for(int i = 0; i < sizeof(collision.blocks) / sizeof(collision.blocks[0]); ++i) {
                CollidableBlock currentBlock = collision.blocks[i];

                if(collision.didCollide(position_x, position_y, width, height, currentBlock)) {
                    collisionResponse(position_x, position_y, width, height, currentBlock, currentEnemy);
                }
            }
        }

        void collisionResponse(float x, float y, float width, float height, CollidableBlock currentBlock, EnemyData* currentEnemy) {
            // referse to enemy bottom and top and left and right
            float bottom = (float)abs(y - (currentBlock.position_y + currentBlock.height));
            float top = (float)abs((y + height) - currentBlock.position_y);

            float left = (float)abs(x - (currentBlock.position_x + currentBlock.width));
            float right = (float)abs((x + width) - currentBlock.position_x);

            CollisionInfo xInfo = {collision.getCollideAxisX(left, right), collision.getCollideAxisX(left, right) == (char*)"left" ? left : right};
            CollisionInfo yInfo = {collision.getCollideAxisY(bottom, top), collision.getCollideAxisY(bottom, top) == (char*)"bottom" ? bottom : top};

            CollisionInfo final = {xInfo.overlap < yInfo.overlap ? xInfo.side : yInfo.side, xInfo.overlap < yInfo.overlap ? xInfo.overlap : yInfo.overlap};

            if(final.side == (char*)"bottom") {
                currentEnemy -> speed.y = 0.0f;

                float newY = currentBlock.position_y + currentBlock.height - currentEnemy -> hitbox.y_offset;

                currentEnemy -> model = glm::translate(currentEnemy -> model, glm::vec3(currentEnemy -> x, newY, 0.0f));

                currentEnemy -> model = glm::translate(currentEnemy -> model, glm::vec3(-currentEnemy -> x, -currentEnemy -> y, 0.0f));

                // currentEnemy -> x = x;
                currentEnemy -> y = newY;

                currentEnemy -> hitbox.updateAxis(currentEnemy -> x, currentBlock.position_y + currentBlock.height);

                // setPosition(x, currentBlock.position_y + currentBlock.height - hitbox.y_offset);
                // hitbox.updateAxis(x, currentBlock.position_y + currentBlock.height);
            }
            if(final.side == (char*)"right") {
                currentEnemy -> speed.x = 0.0f;

                float newX = currentBlock.position_x - width - currentEnemy -> hitbox.x_offset;

                currentEnemy -> model = glm::translate(currentEnemy -> model, glm::vec3(newX, currentEnemy -> y, 0.0f));

                currentEnemy -> model = glm::translate(currentEnemy -> model, glm::vec3(-currentEnemy -> x, -currentEnemy -> y, 0.0f));

                currentEnemy -> x = newX;

                currentEnemy -> hitbox.updateAxis(currentBlock.position_x - width, currentEnemy -> y);

                // setPosition(currentBlock.position_x - width - hitbox.x_offset, y);
                // hitbox.updateAxis(currentBlock.position_x - width, y);
            }
        }

        void update() {
            if(elapsedFrames % spawnBuffer == 0) {
                // addEnemy(300.0f, 500.0f);

                // printf("HELLO\n");

                elapsedFrames = 1;
            }

            // std::cout << sin(glm::radians(sinNumber)) * 5.0f << std::endl;

            float timeTest = sin(glm::radians(sinNumber)) * 5.0f;

            float speedX;

            if(timeTest >= 0.0f) speedX = 1.0f;
            else speedX = -1.0f;
            
            for(int i = 0; i < sizeof(factoryEnemies) / sizeof(EnemyData); ++i) {
                if(factoryEnemies[i].active) {
                    factoryEnemies[i].model = glm::mat4(1.0f);
                    gravity.applyGravity(&factoryEnemies[i].speed);
                    factoryEnemies[i].speed.x = speedX;
                    factoryEnemies[i].x += factoryEnemies[i].speed.x;
                    factoryEnemies[i].y += factoryEnemies[i].speed.y;
                    factoryEnemies[i].model = glm::translate(factoryEnemies[i].model, glm::vec3(factoryEnemies[i].x, factoryEnemies[i].y, 0.0f));
                    factoryEnemies[i].hitbox.updateAxis(factoryEnemies[i].x, factoryEnemies[i].y);
                    // checkWallCollision();
                    checkCollision(
                        factoryEnemies[i].hitbox.position_x, factoryEnemies[i].hitbox.position_y, 
                        factoryEnemies[i].hitbox.width, factoryEnemies[i].hitbox.height,
                        &factoryEnemies[i]
                    );
                    factoryEnemies[i].hitbox.updateAxis(factoryEnemies[i].x, factoryEnemies[i].y);
                    factoryEnemies[i].animation.animate();
                }
            }
            ++sinNumber;
            ++elapsedFrames;
        }

        void render(Shader* shader, glm::mat4 projection, glm::mat4 view) {
            for(int i = 0; i < sizeof(factoryEnemies) / sizeof(EnemyData); ++i) {
                EnemyData currentEnemy = factoryEnemies[i];
                if(currentEnemy.active) {
                    glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
                    glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
                    glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(currentEnemy.model));
                    setUniform1f(shader, (char*)"totalFrames", currentEnemy.animation.currentAnimatedState[0].totalFrames);
                    setUniform1f(shader, (char*)"currentFrame", currentEnemy.animation.currentFrame);
                    glBindTexture(GL_TEXTURE_2D, *currentEnemy.animation.currentAnimatedState[0].TBO);
                    glBindVertexArray(VAO);
                    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                }
            }
        }
};

#endif