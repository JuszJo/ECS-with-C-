#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "enemy.h"

class EnemyManager {
    public:
        EnemyManager() {}

        static void createEnemy(Shader* shader, float position_x, float position_y) {
            Enemy* newEnemy = new Enemy(shader, 0.0f, 0.0f, 100.0f, 100.0f, currentIndex);

            newEnemy -> x = 200.0f;
            newEnemy -> y = 500.0f;

            newEnemy -> active = true;

            addEntity(newEnemy)
            /* Bullet* newBullet = new Bullet(shader, 0.0f, 0.0f, 20.0f, 20.0f, currentIndex);

            newBullet -> x = position_x;
            newBullet -> y = position_y;

            newBullet -> active = true;

            addEntity(newBullet); */
        }
};


#endif