#ifndef ANIMATION_H
#define ANIMATION_H

struct AnimatedState {
    char* name;
    float totalFrames;
    int animateBuffer;
    int textureIndex;
    unsigned int* TBO;
    bool reversed;
};

class Animation {
    public:
        AnimatedState currentAnimatedState[1];
        int elapsedFrames = 0;
        int currentFrame = 1;
        bool shouldAnimate = true;

        Animation() {};

        Animation(char* name, float totalFrames, int animateBuffer, int textureIndex, unsigned int* TBO, bool reversed) {
            AnimatedState currentAnimation = {name, totalFrames, animateBuffer, textureIndex, TBO, reversed};

            currentAnimatedState[0] = currentAnimation;
        }

        void updateTextureBuffer(unsigned int* TBO) {
            currentAnimatedState[0].TBO = TBO;
        }

        void resetAnimation(AnimatedState currentAnimation) {
            elapsedFrames = 0;
            currentFrame = currentAnimation.reversed ? currentAnimation.totalFrames + 1 : 0;
        }

        void setCurrentAnimation(char* name, float totalFrames, int animateBuffer, int textureIndex, unsigned int* textureBuffer, bool reversed) {
            if(currentAnimatedState[0].name != name) {
                AnimatedState animatedState;
                animatedState.name = name;
                animatedState.totalFrames = totalFrames;
                animatedState.animateBuffer = animateBuffer;
                animatedState.textureIndex = textureIndex;
                animatedState.TBO = textureBuffer;
                animatedState.reversed = reversed;

                currentAnimatedState[0] = animatedState;

                resetAnimation(currentAnimatedState[0]);
            }
        }

        void animateForward(AnimatedState currentAnimation) {
            if(currentFrame >= currentAnimation.totalFrames) currentFrame = 1;
            else ++currentFrame;
        }

        void animateReversed(AnimatedState currentAnimation) {
            if(currentFrame <= 1) currentFrame = currentAnimation.totalFrames;
            else --currentFrame;
        }

        void animate() {
            if(shouldAnimate) {
                if(elapsedFrames % currentAnimatedState[0].animateBuffer == 0) {
                    if(!currentAnimatedState[0].reversed) animateForward(currentAnimatedState[0]);
                    else animateReversed(currentAnimatedState[0]);

                    elapsedFrames = 0;
                }

                ++elapsedFrames;
            }
            else {
                resetAnimation(currentAnimatedState[0]);
            }
        }
};

#endif