#pragma once
#include <glm/glm.hpp>
#include "window.h"

class Camera
{
public:
	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;

	glm::vec3 lookAtPoint;

	enum CameraMode
	{
		FixedOrbit,
		FreeOrbit,
		Normal
	};

	struct Move
	{
		glm::vec2 dir;
	};

	CameraMode mode = CameraMode::FixedOrbit;

	float cameraSensitivity = 0.6f;
	float cameraPanSensitivity = 0.05f;
	float cameraZoomSensitivity = 0.7f;
	float cameraZoomSpeed = 15;
	float cameraZoomDeceleration = 250;

	float cameraSpeed = 15;

	float cameraDistance = 5;
	float cameraMinDistance = 1;
	float cameraMaxDistance = 25;

	float cameraPitch;
	float cameraYaw;

	Camera(Window* window, glm::vec3 up, glm::vec3 lookAt);
	~Camera();

	void createLookAt();
	void update(float dt);
	void pan(float dt);
	void orbit(float dt);
	void zoom(float dt);
	void move(float dt, Move move);

private:
	int centerX;
	int centerY;
	float camZoomSpeed;
	Window* window;
	double mouseX;
	double mouseY;
	double prevMouseX;
	double prevMouseY;
	Move currentMove;

};