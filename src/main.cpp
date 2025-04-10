﻿#include "engine/config/config.h"
#include "engine/config/essentials.h"

int main() {
    yume::setWindowSize(1280, 720);
    yume::initWindow("YUME");

    glEnable(GL_DEPTH_TEST);

    while(yume::isWindowOpen()) {
        yume::updateWindow();

		if (yume::keyPressed(KEY_ESCAPE)) {
			yume::setWindowStatus(false);
		}

		yume::updateInput(yume::getWindowPointer());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        yume::swapBuffersPollEvents();
    }

    yume::closeWindow();
}