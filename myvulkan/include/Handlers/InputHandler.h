#pragma once
#include "windowHandler.h"

void mouseCallbackHelper(GLFWwindow*, double, double);
void keyCallbackHelper(GLFWwindow*, int, int, int, int);

class InputHandler {
private:
	float lastX = WIDTH / 2.0f;
	float lastY = HEIGHT / 2.0f;

public:
	InputHandler();
	~InputHandler();

	void processMovement();
	void processKey(int, int);
	void processMouseMovement(float xoffset, float yoffset);


	bool mouseUsed = false;
};

