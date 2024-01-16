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
            for(int i = 0; i < size; ++i) {
                if(entityList[i] -> active) {
                    entityList[i] -> listen(&keyInput.key);
                }
            }
        }
};

#endif