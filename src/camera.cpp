#include "camera.h"
#include "window.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtx/quaternion.hpp>

#define PI 3.14159265

Camera::Camera(Window* window, glm::vec3 up, glm::vec3 lookAt)
{
	this->up = up;
	this->window = window;
	this->lookAtPoint = lookAt;

	centerX = window->width / 2;
	centerY = window->height / 2;

	mouseX = centerX;
	mouseY = centerY;

	currentMove.dir = glm::vec2(0);

	createLookAt();
	projection = glm::perspective(3.1415926f / 4, (float)window->width / window->height, 0.5f, 300.0f);

	window->mouseMoveEventHandlers.push_back(
		[this](Window* wnd, double x, double y)
		{
			prevMouseX = mouseX;
			prevMouseY = mouseY;
			mouseX = x;
			mouseY = y;
		});

	window->keyEventHandlers.push_back(
		[this](Window* wnd, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				if (key == GLFW_KEY_W)
					this->currentMove.dir.y++;
				if (key == GLFW_KEY_S)
					this->currentMove.dir.y--;
				if (key == GLFW_KEY_A)
					this->currentMove.dir.x--;
				if (key == GLFW_KEY_D)
					this->currentMove.dir.x++;
			}
			if (action == GLFW_RELEASE)
			{
				if (key == GLFW_KEY_W)
					this->currentMove.dir.y--;
				if (key == GLFW_KEY_S)
					this->currentMove.dir.y++;
				if (key == GLFW_KEY_A)
					this->currentMove.dir.x++;
				if (key == GLFW_KEY_D)
					this->currentMove.dir.x--;
			}
		}
	);
}

Camera::~Camera()
{
}

void Camera::createLookAt()
{
	view = glm::lookAt(position, lookAtPoint, up);
}

void Camera::update(float dt)
{
	createLookAt();
	orbit(dt);
	move(dt, currentMove);
}

void Camera::pan(float dt)
{
	glm::vec2 deltaPos(centerX - mouseX, centerY - mouseY);

	glm::vec3 dir = glm::proj(direction, up);

	glm::vec3 sideways = glm::normalize(glm::cross(dir, up));

	glm::vec3 forwardvec = -cameraPanSensitivity * deltaPos.x * dir;
	glm::vec3 sidewaysvec = -cameraPanSensitivity * deltaPos.y * sideways;

	glm::vec3 resultant = forwardvec + sidewaysvec;

	window->setCursorPosition(centerX, centerY);

	lookAtPoint += resultant;
	position += resultant;
}

void Camera::orbit(float dt)
{
	glm::vec2 deltaPos(centerX - mouseX, centerY - mouseY);
	window->setCursorPosition(centerX, centerY);

	float rotX = 2 * PI * cameraSensitivity * deltaPos.x * 0.001f;
	float rotY = 2 * PI * cameraSensitivity * deltaPos.y * 0.001f;

	cameraYaw -= rotX;
	cameraPitch += rotY;

	cameraYaw = fmodf(cameraYaw, 2 * PI);

	if (rotX != 0 || rotY != 0 || camZoomSpeed != 0)
	{
		glm::vec3 relcamdir = lookAtPoint - position;
		glm::vec3 relcampos = relcamdir;
		relcamdir = glm::normalize(relcamdir);

		switch (mode)
		{
			case FreeOrbit:
			{
				glm::quat rot1 = glm::angleAxis(rotX, up);
				glm::vec3 otherdir = glm::normalize(glm::cross(relcamdir, up));

				glm::quat rot2 = glm::angleAxis(rotY, otherdir);

				rot1 *= rot2;

				glm::mat4 rotmatrix = glm::mat4_cast(rot1);

				glm::vec3 newrelpos = glm::normalize(rotmatrix * glm::vec4(relcampos, 0));
				direction = newrelpos;

				newrelpos *= cameraDistance;

				position = lookAtPoint - newrelpos;

				up = glm::normalize(glm::cross(otherdir, newrelpos));
			}
				break;
					
			case FixedOrbit:
			{
				cameraPitch = fmaxf(-PI / 2, fminf(PI / 2 - 0.001, cameraPitch));

				glm::vec3 directionVector(1, 0, 0);
				glm::quat q1 = glm::angleAxis(cameraPitch, glm::vec3(0, 1, 0));
				directionVector = glm::vec4(directionVector.x, directionVector.y, directionVector.z, 0) * glm::mat4_cast(q1);
				glm::quat q2 = glm::angleAxis(cameraYaw, glm::vec3(0, 0, 1));
				directionVector = glm::vec4(directionVector.x, directionVector.y, directionVector.z, 0) * glm::mat4_cast(q2);

				directionVector *= cameraDistance;

				position = lookAtPoint - directionVector;

				direction = directionVector;
			}
				break;

			case Normal:
			{
				cameraPitch = fmaxf(-PI / 2, fminf(PI / 2 - 0.001, cameraPitch));

				glm::quat rot1 = glm::angleAxis(rotX, up);
				glm::vec3 otherdir = glm::normalize(glm::cross(relcamdir, up));

				glm::quat rot2 = glm::angleAxis(rotY, otherdir);

				rot1 *= rot2;

				glm::mat4 rotmatrix = glm::mat4_cast(rot1);

				glm::vec3 newrelpos = glm::normalize(rotmatrix * glm::vec4(relcampos, 0));
				direction = newrelpos;
				lookAtPoint = position + direction;
			}
				break;
		}
	}

	
}

void Camera::zoom(float dt)
{
}

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

void Camera::move(float dt, Move move)
{
	glm::vec3 fwd = dt * cameraSpeed * sgn(move.dir.y) * direction;
	this->lookAtPoint += fwd;
	this->position += fwd;

	glm::vec3 sidewaysdir = glm::normalize(glm::cross(direction, up));
	glm::vec3 sideways = dt * cameraSpeed * sgn(move.dir.x) * sidewaysdir;

	this->lookAtPoint += sideways;
	this->position += sideways;
}