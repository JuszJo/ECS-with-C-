#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "bullet.h"

class BulletManager {
    public:
        static inline Shader* bulletShader = nullptr;
        static float width;
        static float height;

        BulletManager() {}

        static void createBullet(Shader* shader, float position_x, float position_y, const char* bullet_color, bool reverseDirection = false) {
            // std::cout << "active bullets: " << activeBullets << std::endl;
            if(activeBullets < maxActiveBullets) {
                Bullet* newBullet = new Bullet(bulletShader, 0.0f, 0.0f, width, height);

                if(bullet_color == "blue") {
                    newBullet->color[0] = 0.0f;
                    newBullet->color[1] = 0.5f;
                    newBullet->color[2] = 1.0f;
                }
                else {
                    newBullet->color[0] = 1.0f;
                    newBullet->color[1] = 0.0f;
                    newBullet->color[2] = 0.0f;
                }

                newBullet->x = position_x;
                newBullet->y = position_y;

                if(reverseDirection) newBullet->acceleration *= -1.0f;

                newBullet->active = true;

                entityManager.addEntity(newBullet);

                ++activeBullets;
            }
        }

        static void createDualBullet(Shader* shader, float position_x, float position_y, const char* bullet_color, bool reverseDirection = false) {
            if(activeBullets < maxActiveBullets) {
                Bullet* newBullet = new Bullet(bulletShader, 0.0f, 0.0f, width, height);

                if(bullet_color == "blue") {
                    newBullet->color[0] = 0.0f;
                    newBullet->color[1] = 0.5f;
                    newBullet->color[2] = 1.0f;
                }
                else {
                    newBullet->color[0] = 1.0f;
                    newBullet->color[1] = 0.0f;
                    newBullet->color[2] = 0.0f;
                }

                newBullet->x = position_x - 20.0f;
                newBullet->y = position_y;

                if(reverseDirection) newBullet->acceleration *= -1.0f;

                newBullet->active = true;

                Bullet* newBullet2 = new Bullet;

                *newBullet2 = *newBullet;

                newBullet2->x = position_x + 20.0f;

                entityManager.addEntity(newBullet);
                entityManager.addEntity(newBullet2);

                ++activeBullets;
                ++activeBullets;
            }
        }

        static void removeAllBullets() {
            for(int i = 0; i < entityManager.entity_list.size(); ++i) {
                if(entityManager.entity_list[i] -> name == (char*)"bullet") {
                    entityManager.entity_list[i] -> active = false;
                }
            }
        }
};

float BulletManager::width = 10.0f;
float BulletManager::height = 30.0f;

#endif