// Specify numer of frames before callback runs and
// return a bool signifying whether the callback has ran
bool pseudo_async(int frameBuffer, void(*callback)()) {
    bool finished = false;
    static int elapsedFrames = 1;

    if(elapsedFrames % frameBuffer == 0) {
        elapsedFrames = 0;

        callback();

        finished = true;
    }

    ++elapsedFrames;

    return finished;
}