#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

class Camera {
	glm::vec3 position;
	glm::vec3 viewDirection;
public:
	Camera();
	glm::mat4 worldToView() const;
	void moveY(float distance) const;
};