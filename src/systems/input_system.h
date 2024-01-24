#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "../keyinput.h"
#include "../entityV2.h"

class InputSystem {
    public:
        KeyInput keyInput;

        InputSystem() {}

        void processInput(GLFWwindow* window) {
            keyInput.processInput(window);
        }

        void listen() {
            for(int i = 0; i < entityManager.entity_list.size(); ++i) {
                if(entityManager.entity_list[i]->active) {
                    entityManager.entity_list[i]->listen(&keyInput.key);
                }
            }
        }
};

#endif