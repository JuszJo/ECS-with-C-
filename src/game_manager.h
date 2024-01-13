#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "entity_manager.h"

class GameManager {
    public:
        EntityManager* entityManager;
        
        GameManager() {}
};

#endif