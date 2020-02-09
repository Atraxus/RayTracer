#include "Camera.h"

Camera::Camera():
	m_ViewDirection(0.0f, 0.0f, 0.0f),
	m_Position(0.0f, 0.0f, 0.0f)
{
	m_Fov = glm::radians(45.0f);
}
Camera::Camera(glm::vec3 positionInput, glm::vec3 viewInput)
{
	m_Position = positionInput;
	m_ViewDirection = viewInput;
	m_Fov = glm::radians(45.0f);
}

glm::mat4 Camera::getView() const {
	glm::mat4 view = glm::lookAt(
		m_Position, // Camera is at (4,3,3), in World Space
		m_ViewDirection, // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	return view;
}
glm::mat4 Camera::getProj()
{
	return glm::perspective(m_Fov, 1.33f, 0.1f, 100.f);
}
void Camera::setFov(float degrees) {
	m_Fov = glm::radians(degrees);
}
void Camera::setPosition(glm::vec3 inputPosition){
	m_Position = inputPosition;
}