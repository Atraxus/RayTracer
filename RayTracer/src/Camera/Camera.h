#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

class Camera {
public:
	Camera();
	Camera(glm::vec3 positionInput, glm::vec3 viewInput);
	glm::mat4 getView() const;
	glm::mat4 getProj();
	void setFov(float inputFov);
	void setPosition(glm::vec3 inputPosition);
private:
	glm::vec3 m_Position;
	glm::vec3 m_ViewDirection;
	float m_Fov;
};