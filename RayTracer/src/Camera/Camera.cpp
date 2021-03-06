#include "Camera.h"

Camera::Camera(int width, int height, float fovDeg, glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up) 
	: m_Width(width), m_Height(height), m_Position(pos), m_Fov(glm::radians(45.0f)), m_LookAt(lookAt), m_HeadUp(up)
{
	m_ViewDirection = glm::normalize(lookAt - pos);
	m_Right = glm::normalize(glm::cross(up, m_ViewDirection));
	m_Up = glm::cross(m_Right, m_ViewDirection);

	m_FovY = tan(glm::radians(fovDeg));
	m_FovX = (static_cast<float>(m_Width)* m_FovY) / static_cast<float>(m_Height);
}

void Camera::setFov(float degrees) {
	m_Fov = glm::radians(degrees);
}

void Camera::setPosition(glm::vec3 inputPosition){
	m_Position = inputPosition;
}

void Camera::setLookAt(glm::vec3 lookAt) {
	m_LookAt = lookAt;
	m_ViewDirection = glm::normalize(lookAt - m_Position);
	m_Right = glm::normalize(glm::cross(m_HeadUp, m_ViewDirection));
	m_Up = glm::cross(m_Right, m_ViewDirection);
}

void Camera::setViewDirection(glm::vec3 viewDir)
{
	m_ViewDirection = glm::normalize(viewDir);
	m_Right = glm::normalize(glm::cross(m_HeadUp, m_ViewDirection));
	m_Up = glm::cross(m_Right, m_ViewDirection);
}

glm::mat4 Camera::getView() const {
	glm::mat4 view = glm::lookAt(
		m_Position, // Camera is at (4,3,3), in World Space
		m_ViewDirection, // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	return view;
}
