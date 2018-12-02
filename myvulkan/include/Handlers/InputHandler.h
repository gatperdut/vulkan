#pragma once


#include "Window/window.h"

void mouseCallbackHelper(GLFWwindow*, double, double);
void keyCallbackHelper(GLFWwindow*, int, int, int, int);

class InputHandler {
private:
	float lastX = window::WIDTH / 2.0f;
	float lastY = window::HEIGHT / 2.0f;

public:
	InputHandler();
	~InputHandler();

	void processMovement();
	void processKey(int, int);
	void processMouseMovement(float xoffset, float yoffset);


	bool mouseUsed = false;
};

