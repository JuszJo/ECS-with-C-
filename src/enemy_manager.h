#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "enemy.h"

class EnemyManager {
    public:
        EnemyManager() {}

        static inline Shader* shader = nullptr;

        static inline float enemyStartingPositionY = 530.0f;
        static inline int enemyCount = 0;
        static inline int spawnBuffer = 100;
        static inline int elapsedSpawnFrames = 1;

        static void resetSettings() {
            enemyCount = 0;
            elapsedSpawnFrames = 1;
        }

        static void createEnemy(float position_x, float position_y) {
            Enemy* newEnemy = new Enemy(shader, 0.0f, 0.0f, 50.0f, 50.0f);

            newEnemy->x = position_x;
            newEnemy->y = position_y;

            newEnemy->active = true;

            entityManager.entity_list.add_element(newEnemy);

            ++enemyCount;
        }

        static void createMulitipleEnemies(float position_x, float position_y, int amount) {
            for(int i = 0; i < amount; ++i) {
                createEnemy(position_x + (i * 80.0f), position_y);
            }
        }

        static void removeAllEnemies() {
            for(int i = 0; i < entityManager.entity_list.size(); ++i) {
                if(entityManager.entity_list[i] -> name == (char*)"enemy") {
                    entityManager.entity_list[i] -> active = false;
                }
            }
        }

        static void reduceMaxShootBuffer() {
            for(int i = 0; i < entityManager.entity_list.size(); ++i) {
                if(entityManager.entity_list[i]->active && entityManager.entity_list[i]->name == (char*)"enemy") {
                    entityManager.entity_list[i]->performAction((char*)"reduce_max_shoot_buffer");
                }
            }
        }

        static void reduceEnemyCount() {
            --enemyCount;
        }
};


#endif