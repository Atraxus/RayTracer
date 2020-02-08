#include "Camera.h"

Camera::Camera():
	viewDirection(0.0f, 0.0f, -1.0f),
	position(0.0f, 0.0f, 0.0f)
{
	fov = glm::radians(45.0f);
}
Camera::Camera(glm::vec3 positionInput, glm::vec3 viewInput)
{
	position = positionInput;
	viewDirection = viewInput;
	fov = glm::radians(45.0f);
}

glm::mat4 Camera::getView() const {
	glm::mat4 view = glm::lookAt(
		position, // Camera is at (4,3,3), in World Space
		viewDirection, // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	return view;
}
glm::mat4 Camera::getProj()
{
	return glm::perspective(fov, 1.33f, 0.1f, 100.f);
}
void Camera::setFov(float degrees) {
	fov = glm::radians(degrees);
}
void Camera::setPosition(glm::vec3 inputPosition){
	position = inputPosition;
}