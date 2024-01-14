#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "entity_manager.h"
#include "bullet.h"

class GameManager {

    public:
        // static EntityManager* entityManager;
        
        GameManager() {}

        static void createBullet() {
            Bullet bullet(300.0f, 200.0f, 50.0f, 50.0f);
            bullet.active = true;

            // entityManager -> addEntity(&bullet);
        }
};

#endif