#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "bullet.h"

class BulletManager {
    public:
        BulletManager() {}

        static void createBullet(Shader* shader, float position_x, float position_y) {
            if(activeBullets < maxActiveBullets) {
                Bullet* newBullet = new Bullet(shader, 0.0f, 0.0f, 20.0f, 20.0f, currentIndex);

                newBullet -> x = position_x;
                newBullet -> y = position_y;

                newBullet -> active = true;

                addEntity(newBullet);

                ++activeBullets;
            }
        }
};


#endif