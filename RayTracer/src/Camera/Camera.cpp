#include "Camera.h"

Camera::Camera(int width, int height, float fovY, glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up) 
	: m_Width(width), m_Height(height), m_Position(pos), m_Fov(glm::radians(45.0f))
{
	m_ViewDirection = glm::normalize(lookAt - pos);
	m_Right = glm::normalize(glm::cross(up, m_ViewDirection));
	m_Up = glm::cross(m_Right, m_ViewDirection);

	m_FovY = std::tan(fovY * std::acos(-1) / 180.f / 2.0f);
	m_FovX = (static_cast<float>(m_Width)* m_FovY) / static_cast<float>(m_Height);
}

glm::mat4 Camera::getView() const {
	glm::mat4 view = glm::lookAt(
		m_Position, // Camera is at (4,3,3), in World Space
		m_ViewDirection, // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	return view;
}

void Camera::setFov(float degrees) {
	m_Fov = glm::radians(degrees);
}

void Camera::setPosition(glm::vec3 inputPosition){
	m_Position = inputPosition;
}