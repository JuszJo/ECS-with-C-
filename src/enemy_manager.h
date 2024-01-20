#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "enemy.h"

class EnemyManager {
    public:
        EnemyManager() {}

        static void createEnemy(Shader* shader, float position_x, float position_y) {
            Enemy* newEnemy = new Enemy(shader, 0.0f, 0.0f, 50.0f, 50.0f, currentIndex);

            newEnemy -> x = position_x;
            newEnemy -> y = position_y;

            newEnemy -> active = true;

            addEntity(newEnemy);

            ++enemyCount;
        }

        static void createMulitipleEnemies(Shader* shader, float position_x, float position_y, int amount) {
            for(int i = 0; i < amount; ++i) {
                createEnemy(shader, position_x + (i * 50.0f), position_y);
            }
        }
};


#endif