#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "bullet.h"

class BulletManager {
    public:
        static float width;
        static float height;

        BulletManager() {}

        static void createBullet(Shader* shader, float position_x, float position_y, bool reverseDirection = false) {
            // std::cout << "active bullets: " << activeBullets << std::endl;
            if(activeBullets < maxActiveBullets) {
                Bullet* newBullet = new Bullet(shader, 0.0f, 0.0f, width, height);

                newBullet->x = position_x;
                newBullet->y = position_y;

                if(reverseDirection) newBullet->acceleration *= -1.0f;

                newBullet->active = true;

                entityManager.addEntity(newBullet);

                ++activeBullets;
            }
        }
};

float BulletManager::width = 10.0f;
float BulletManager::height = 10.0f;

#endif