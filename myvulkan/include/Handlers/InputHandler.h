#pragma once
#include "WindowHandler.h"

void mouseCallbackHelper(GLFWwindow*, double, double);


class InputHandler {
private:
	float lastX = WIDTH / 2.0f;
	float lastY = HEIGHT / 2.0f;

public:
	InputHandler();
	~InputHandler();

	void processKeyboard();
	void processMouseMovement(float xoffset, float yoffset);


	bool mouseUsed = false;
};

