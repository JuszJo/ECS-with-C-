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

        void listen(EntityV2** entityPointer, int arraySize) {
            for(int i = 0; i < arraySize; ++i) {
                if(entityPointer[i] -> active) {
                    entityPointer[i] -> listen(&keyInput.key);
                }
            }
        }
};

#endif